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
	};
}