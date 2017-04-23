#include <memory>

#include "TextureResourceLoaderImpl.h"
#include "WindowsUtil.h"
#include "DirectXState.h"

#include "../VeritasEngineBase/ResourceHandle.h"

#include "DDSTextureLoader/DDSTextureLoader.h"

#include "DirectXTextureData.h"

VeritasEngine::TextureResourceLoaderImpl::TextureResourceLoaderImpl(std::shared_ptr<DirectXState> dxState)
	: m_dxState{ dxState }
{
	
}


void VeritasEngine::TextureResourceLoaderImpl::Load(std::istream& data, ResourceHandle& handle) const
{
	data.seekg(0, std::ios::end);

	size_t length = static_cast<size_t>(data.tellg());

	data.seekg(0, std::ios::beg);

	uint8_t* img = new uint8_t[length];

	data.read(reinterpret_cast<char*>(img), length);

	ComPtr<ID3D11Resource> texture;
	ComPtr<ID3D11ShaderResourceView> textureView;
	HHR(DirectX::CreateDDSTextureFromMemory(m_dxState->Device.Get(), m_dxState->Context.Get(), img, length, texture.GetAddressOf(), textureView.GetAddressOf()), "Failed to load texture");

	delete[] img;

	handle.SetData(DirectXTextureData{ texture, textureView });
}
