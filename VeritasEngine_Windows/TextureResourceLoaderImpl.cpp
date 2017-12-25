#include <memory>

#include "TextureResourceLoaderImpl.h"
#include "WindowsUtil.h"
#include "DirectXState.h"

#include "DDSTextureLoader/DDSTextureLoader.h"

#include "DirectXTextureData.h"
#include "../VeritasEngine/ResourceData.h"

VeritasEngine::TextureResourceLoaderImpl::TextureResourceLoaderImpl(std::shared_ptr<DirectXState> dxState)
	: m_dxState{ std::move(dxState) }
{
	
}


void VeritasEngine::TextureResourceLoaderImpl::Load(std::istream& data, ResourceData& handle) const
{
	data.seekg(0, std::ios::end);

	const auto length = static_cast<size_t>(data.tellg());

	data.seekg(0, std::ios::beg);

	std::unique_ptr<uint8_t[]> img(new uint8_t[length]);

	data.read(reinterpret_cast<char*>(img.get()), length);

	ComPtr<ID3D11Resource> texture;
	ComPtr<ID3D11ShaderResourceView> textureView;
	DirectX::DDS_ALPHA_MODE mode;
	HHR(DirectX::CreateDDSTextureFromMemory(m_dxState->Device.Get(), m_dxState->Context.Get(), img.get(), length, texture.GetAddressOf(), textureView.GetAddressOf(), 0, &mode), "Failed to load texture");

	handle.SetData(DirectXTextureData{ texture, textureView });
}
