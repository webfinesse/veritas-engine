#pragma once

#include <wrl\client.h>
#include <d3d11.h>

using namespace Microsoft::WRL;

namespace VeritasEngine
{
	class DirectXState
	{
	public:
		ComPtr<ID3D11Device> Device{ nullptr };
		ComPtr<IDXGISwapChain> SwapChain{ nullptr };
		ComPtr<ID3D11DeviceContext> Context{ nullptr };
		ComPtr<ID3D11RenderTargetView> RenderTargetView{ nullptr };
		ComPtr<ID3D11DepthStencilView> DepthStencilView{ nullptr };

		~DirectXState()
		{
			ID3D11RenderTargetView* nullViews[] = { nullptr };
			Context->OMSetRenderTargets(ARRAYSIZE(nullViews), nullViews, nullptr);

			DepthStencilView = nullptr;
			RenderTargetView = nullptr;
			Context = nullptr;
			SwapChain = nullptr;

#if defined(_DEBUG)
			ComPtr<ID3D11Debug> debugDevice = nullptr;
			Device.As(&debugDevice);

			Device = nullptr;

			//debugDevice->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL); // uncomment when tracking down directx leaks
			debugDevice = nullptr;
#else
			g_device = nullptr;
#endif
		}
	};
}