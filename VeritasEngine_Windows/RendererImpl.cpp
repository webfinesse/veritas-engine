#include "RendererImpl.h"

#include <wrl\client.h>
#include <d3d11.h>
#include "WindowsUtil.h"
#include "DirectXState.h"

#include "../VeritasEngineBase/MathTypes.h"
#include "../VeritasEngine/MeshInstance.h"
#include "../VeritasEngine/MeshSubset.h"
#include "../VeritasEngine/IIndexBuffer.h"
#include "../VeritasEngine/IVertexBuffer.h"
#include "../VeritasEngine/FrameDescription.h"
#include "IMeshShader.h"
#include "../VeritasEngine/PerObjectBuffer.h"

using namespace Microsoft::WRL;

struct VeritasEngine::RendererImpl::Impl
{
public:
	Impl(std::shared_ptr<DirectXState> dxState, std::shared_ptr<IMeshShader> meshShader)
		: m_dxState { std::move(dxState) }, m_meshShader{ std::move(meshShader) }
	{

	
	}

	void Init(void* osData, unsigned int bufferWidth, unsigned int bufferHeight)
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

		auto hr = D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, creationFlags, featureLevels, ARRAYSIZE(featureLevels), D3D11_SDK_VERSION, m_dxState->Device.GetAddressOf(), nullptr, m_dxState->Context.GetAddressOf());

		HHR(hr, "Error Creating D3D11 Device");

		ComPtr<IDXGIDevice1> dxgiDevice;
		HHR(m_dxState->Device.As(&dxgiDevice), "Error getting infrastructure device");

		ComPtr<IDXGIAdapter> dxgiAdapter;
		HHR(dxgiDevice->GetAdapter(dxgiAdapter.GetAddressOf()), "Error getting infrastructure adapter");

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

		HHR(dxgiFactory->CreateSwapChain(m_dxState->Device.Get(), &desc, m_dxState->SwapChain.GetAddressOf()), "Error creating swap chain");

		HHR(dxgiDevice->SetMaximumFrameLatency(1), "Error setting maximum frame latency");

		SetupBuffers(bufferWidth, bufferHeight);

		D3D11_RASTERIZER_DESC rasterDesc;
		ZeroMemory(&rasterDesc, sizeof(rasterDesc));
		rasterDesc.CullMode = D3D11_CULL_BACK;
		rasterDesc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;

		ComPtr<ID3D11RasterizerState> resultRasterizer;
		m_dxState->Device->CreateRasterizerState(&rasterDesc, resultRasterizer.GetAddressOf());

		m_dxState->Context->RSSetState(resultRasterizer.Get());

		m_meshShader->Init();
	}

	void Resize(unsigned int bufferWidth, unsigned int bufferHeight)
	{
		// Clear the previous window size specific context.
		ID3D11RenderTargetView* nullViews[] = { nullptr };
		m_dxState->Context->OMSetRenderTargets(ARRAYSIZE(nullViews), nullViews, nullptr);
		m_dxState->RenderTargetView.Reset();
		m_dxState->DepthStencilView.Reset();
		m_dxState->Context->Flush();

		m_dxState->SwapChain->ResizeBuffers(1, bufferWidth, bufferHeight, DXGI_FORMAT::DXGI_FORMAT_UNKNOWN, 0);

		SetupBuffers(bufferWidth, bufferHeight);
	}

	void Present()
	{
		HHR(m_dxState->SwapChain->Present(0, 0), "Error swapping swap chain");
	}

	void SetupBuffers(unsigned int bufferWidth, unsigned int bufferHeight)
	{
		m_aspectRatio = bufferWidth / static_cast<float>(bufferHeight);

		ComPtr<ID3D11Texture2D> backBuffer;
		HHR(m_dxState->SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(backBuffer.GetAddressOf())), "error getting the backbuffer");

		HHR(m_dxState->Device->CreateRenderTargetView(backBuffer.Get(), nullptr, m_dxState->RenderTargetView.GetAddressOf()), "Error creating the render target view");

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

		HHR(m_dxState->Device->CreateTexture2D(&depthDesc, nullptr, depthStencilBuffer.GetAddressOf()), "Error creating stencil buffer texture");
		HHR(m_dxState->Device->CreateDepthStencilView(depthStencilBuffer.Get(), nullptr, m_dxState->DepthStencilView.GetAddressOf()), "Error creating depth stencil view");

		m_dxState->Context->OMSetRenderTargets(1, m_dxState->RenderTargetView.GetAddressOf(), m_dxState->DepthStencilView.Get());

		D3D11_TEXTURE2D_DESC backBufferDesc{ 0 };
		backBuffer->GetDesc(&backBufferDesc);

		D3D11_VIEWPORT viewport{ 0.0f, 0.0f, static_cast<float>(backBufferDesc.Width), static_cast<float>(backBufferDesc.Height), 0.0f, 1.0f };

		m_dxState->Context->RSSetViewports(1, &viewport);
	}

	void Clear()
	{
		float clearColor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
		m_dxState->Context->ClearRenderTargetView(m_dxState->RenderTargetView.Get(), clearColor);
		m_dxState->Context->ClearDepthStencilView(m_dxState->DepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0xFF);
	}

	void SetVertexBuffer(void* buffer, const unsigned int strides[], const unsigned int offsets[])
	{
		auto* nativeVertexBuffer = static_cast<ID3D11Buffer*>(buffer);

		m_dxState->Context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		m_dxState->Context->IASetVertexBuffers(0, 1, &nativeVertexBuffer, strides, offsets);
	}

	void SetIndexBuffer(void* buffer)
	{
		m_dxState->Context->IASetIndexBuffer(static_cast<ID3D11Buffer*>(buffer), DXGI_FORMAT::DXGI_FORMAT_R32_UINT, 0);
	}

	void DrawIndexed(size_t indexCount, size_t indexOffset, size_t baseVertexIndex)
	{
		m_dxState->Context->DrawIndexed(static_cast<UINT>(indexCount), static_cast<UINT>(indexOffset), static_cast<UINT>(baseVertexIndex));
	}

	void Render(FrameDescription& desc)
	{
		Clear();

		m_meshShader->Activate();
		m_meshShader->SetPassParameters(desc.PassBuffer);

		for(const auto& object : desc.Objects)
		{
			const unsigned int strides[1] = { static_cast<unsigned int>(object.VertexSize) };
			const unsigned int offsets[1] = { 0 };

			SetVertexBuffer(object.VertexBuffer, strides, offsets);
			SetIndexBuffer(object.IndexBuffer);

			m_meshShader->SetPerObjectBuffer(object);

			DrawIndexed(object.IndexIndicies.NumberOfElements, object.IndexIndicies.StartIndex, object.VertexIndicies.StartIndex);
		}

		Present();
	}

	float m_aspectRatio{ 0 };
	std::shared_ptr<DirectXState> m_dxState;
	std::shared_ptr<IMeshShader> m_meshShader;
};

void VeritasEngine::RendererImpl::Init(void* osData, unsigned int bufferWidth, unsigned int bufferHeight)
{
	m_impl->Init(osData, bufferWidth, bufferHeight);
}

void VeritasEngine::RendererImpl::Resize(unsigned int bufferWidth, unsigned int bufferHeight)
{
	m_impl->Resize(bufferWidth, bufferHeight);
}

void VeritasEngine::RendererImpl::Render(FrameDescription& frameDesc)
{
	m_impl->Render(frameDesc);
}

float VeritasEngine::RendererImpl::GetAspectRatio() const
{
	return m_impl->m_aspectRatio;
}

VeritasEngine::RendererImpl::RendererImpl(std::shared_ptr<DirectXState> dxState, std::shared_ptr<IMeshShader> meshShader)
	: m_impl(std::make_unique<Impl>(dxState, meshShader))
{

}

VeritasEngine::RendererImpl::RendererImpl(RendererImpl&& other) noexcept
	: m_impl { std::move(other.m_impl) }
{

}

VeritasEngine::RendererImpl::~RendererImpl() = default;

VeritasEngine::RendererImpl& VeritasEngine::RendererImpl::operator=(RendererImpl&& other) noexcept
{
	if(this != &other)
	{
		m_impl = std::move(other.m_impl);
	}

	return *this;
}
