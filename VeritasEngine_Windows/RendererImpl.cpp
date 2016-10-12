#include "../VeritasEngineBase/RendererImpl.h"

#include <wrl\client.h>
#include <d3d11.h>
#include "WindowsUtil.h"


#include "../VeritasEngineBase/MathTypes.h"

using namespace Microsoft::WRL;

ComPtr<ID3D11Device> g_device{};
ComPtr<IDXGISwapChain> g_swapChain{};
ComPtr<ID3D11DeviceContext> g_context{};
ComPtr<ID3D11RenderTargetView> g_renderTargetView{};
ComPtr<ID3D11DepthStencilView> g_depthStencilView{};

struct VeritasEngine::RendererImpl::Impl
{
public:
	Impl()
	{

	}

	void SetupBuffers(unsigned int bufferWidth, unsigned int bufferHeight)
	{
		ComPtr<ID3D11Texture2D> backBuffer;
		HHR(g_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)backBuffer.GetAddressOf()), "error getting the backbuffer");

		HHR(g_device->CreateRenderTargetView(backBuffer.Get(), nullptr, g_renderTargetView.GetAddressOf()), "Error creating the render target view");

		D3D11_TEXTURE2D_DESC depthDesc;
		depthDesc.Width = bufferWidth;
		depthDesc.Height = bufferHeight;
		depthDesc.MipLevels = 1;
		depthDesc.ArraySize = 1;
		depthDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthDesc.SampleDesc.Count = 1;
		depthDesc.SampleDesc.Quality = 0;
		depthDesc.Usage = D3D11_USAGE_DEFAULT;
		depthDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		depthDesc.CPUAccessFlags = 0;
		depthDesc.MiscFlags = 0;

		ComPtr<ID3D11Texture2D> depthStencilBuffer;

		HHR(g_device->CreateTexture2D(&depthDesc, nullptr, depthStencilBuffer.GetAddressOf()), "Error creating stencil buffer texture");
		HHR(g_device->CreateDepthStencilView(depthStencilBuffer.Get(), nullptr, g_depthStencilView.GetAddressOf()), "Error creating depth stencil view");

		g_context->OMSetRenderTargets(1, g_renderTargetView.GetAddressOf(), g_depthStencilView.Get());

		D3D11_TEXTURE2D_DESC backBufferDesc{ 0 };
		backBuffer->GetDesc(&backBufferDesc);

		D3D11_VIEWPORT viewport{ 0.0f, 0.0f, static_cast<float>(backBufferDesc.Width), static_cast<float>(backBufferDesc.Height), 0.0f, 1.0f };

		g_context->RSSetViewports(1, &viewport);
	}

	~Impl()
	{
		ID3D11RenderTargetView* nullViews[] = { nullptr };
		g_context->OMSetRenderTargets(ARRAYSIZE(nullViews), nullViews, nullptr);

		g_depthStencilView = nullptr;
		g_renderTargetView = nullptr;
		g_context = nullptr;
		g_swapChain = nullptr;

#if defined(_DEBUG)
		ComPtr<ID3D11Debug> debugDevice = nullptr;
		g_device.As(&debugDevice);

		g_device = nullptr;

		//debugDevice->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL); // uncomment when tracking down directx leaks
		debugDevice = nullptr;
#else
		g_device = nullptr;
#endif
	}
};

void VeritasEngine::RendererImpl::Init(void * osData, unsigned int bufferWidth, unsigned int bufferHeight)
{
	HWND windowHandle = static_cast<HWND>(osData);
	int creationFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;

#if defined(_DEBUG)
	creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_0
	};

	auto hr = D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, creationFlags, featureLevels, ARRAYSIZE(featureLevels), D3D11_SDK_VERSION, &g_device, nullptr, &g_context);

	HHR(hr, "Error Creating D3D11 Device");

	ComPtr<IDXGIDevice1> dxgiDevice;
	HHR(g_device.As(&dxgiDevice), "Error getting infrastructure device");

	ComPtr<IDXGIAdapter> dxgiAdapter;
	HHR(dxgiDevice->GetAdapter(&dxgiAdapter), "Error getting infrastructure adapter");

	ComPtr<IDXGIFactory1> dxgiFactory;
	HHR(dxgiAdapter->GetParent(__uuidof(IDXGIFactory1), &dxgiFactory), "Error getting infrastructure factory");

	DXGI_SWAP_CHAIN_DESC desc;
	ZeroMemory(&desc, sizeof(DXGI_SWAP_CHAIN_DESC));

	desc.BufferCount = 1;
	desc.BufferDesc.Width = bufferWidth;
	desc.BufferDesc.Height = bufferHeight;
	desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.BufferDesc.RefreshRate.Numerator = 60;
	desc.BufferDesc.RefreshRate.Denominator = 1;
	desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	desc.OutputWindow = windowHandle;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Windowed = TRUE;

	HHR(dxgiFactory->CreateSwapChain(g_device.Get(), &desc, &g_swapChain), "Error creating swap chain");

	HHR(dxgiDevice->SetMaximumFrameLatency(1), "Error setting maximum frame latency");

	m_impl->SetupBuffers(bufferWidth, bufferHeight);

	D3D11_RASTERIZER_DESC rasterDesc;
	ZeroMemory(&rasterDesc, sizeof(rasterDesc));
	rasterDesc.CullMode = D3D11_CULL_BACK;
	rasterDesc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;

	ComPtr<ID3D11RasterizerState> resultRasterizer;
	g_device->CreateRasterizerState(&rasterDesc, &resultRasterizer);

	g_context->RSSetState(resultRasterizer.Get());
}

void VeritasEngine::RendererImpl::Resize(unsigned int bufferWidth, unsigned int bufferHeight)
{
	// Clear the previous window size specific context.
	ID3D11RenderTargetView* nullViews[] = { nullptr };
	g_context->OMSetRenderTargets(ARRAYSIZE(nullViews), nullViews, nullptr);
	g_renderTargetView.Reset();
	g_depthStencilView.Reset();
	g_context->Flush();

	g_swapChain->ResizeBuffers(1, bufferWidth, bufferHeight, DXGI_FORMAT::DXGI_FORMAT_UNKNOWN, 0);

	m_impl->SetupBuffers(bufferWidth, bufferHeight);
}

void VeritasEngine::RendererImpl::Clear()
{
	float clearColor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	g_context->ClearRenderTargetView(g_renderTargetView.Get(), clearColor);
	g_context->ClearDepthStencilView(g_depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0xFF);
}

void VeritasEngine::RendererImpl::SetVertexBuffer(void* buffer, const unsigned int strides[], const unsigned int offsets[])
{
	auto* nativeVertexBuffer = static_cast<ID3D11Buffer*>(buffer);

	g_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	g_context->IASetVertexBuffers(0, 1, &nativeVertexBuffer, strides, offsets);
}

void VeritasEngine::RendererImpl::SetIndexBuffer(void* buffer)
{
	g_context->IASetIndexBuffer(static_cast<ID3D11Buffer*>(buffer), DXGI_FORMAT::DXGI_FORMAT_R32_UINT, 0);
}

void VeritasEngine::RendererImpl::DrawIndexed(unsigned int indexCount, unsigned int indexOffset, unsigned int baseVertexIndex)
{
	g_context->DrawIndexed(indexCount, indexOffset, baseVertexIndex);
}

void VeritasEngine::RendererImpl::Present()
{
	HHR(g_swapChain->Present(0, 0), "Error swapping swap chain");
}

VeritasEngine::RendererImpl::RendererImpl()
	: m_impl(std::make_unique<Impl>())
{

}

VeritasEngine::RendererImpl::~RendererImpl() = default;
