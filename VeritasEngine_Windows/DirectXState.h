#pragma once

#include <wrl\client.h>
#include <d3d11.h>

using namespace Microsoft::WRL;

namespace VeritasEngine
{
	class DirectXState
	{
	public:
		DirectXState() noexcept = default;
		~DirectXState() noexcept = default;

		DirectXState(DirectXState&& other) noexcept
			: Device{ std::move(other.Device) }, SwapChain{ std::move(other.SwapChain) }, Context{ std::move(other.Context) }, RenderTargetView{ std::move(other.RenderTargetView) }, DepthStencilView{ std::move(other.DepthStencilView) }
		{

		}

		DirectXState& operator=(DirectXState&& other) noexcept
		{
			if(this != &other)
			{
				Device = std::move(other.Device);
				SwapChain = std::move(other.SwapChain);
				Context = std::move(other.Context);
				RenderTargetView = std::move(other.RenderTargetView);
				DepthStencilView = std::move(other.DepthStencilView);
			}

			return *this;
		}

		ComPtr<ID3D11Device> Device{ nullptr };
		ComPtr<IDXGISwapChain> SwapChain{ nullptr };
		ComPtr<ID3D11DeviceContext> Context{ nullptr };
		ComPtr<ID3D11RenderTargetView> RenderTargetView{ nullptr };
		ComPtr<ID3D11DepthStencilView> DepthStencilView{ nullptr };
	};
}