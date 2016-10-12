#pragma once

#include <wrl.h>
#include <d3d11.h>

using namespace Microsoft::WRL;

namespace VeritasEngine
{
	struct DirectXTextureData
	{
		DirectXTextureData(ComPtr<ID3D11Resource> texture, ComPtr<ID3D11ShaderResourceView> textureView)
			: Texture(texture), TextureView(textureView)
		{
			
		}

		ComPtr<ID3D11Resource> Texture;
		ComPtr<ID3D11ShaderResourceView> TextureView;
	};
}
