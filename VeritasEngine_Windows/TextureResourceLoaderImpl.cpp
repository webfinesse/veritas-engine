#include <memory>

#include "../VeritasEngineBase/TextureResourceLoaderImpl.h"
#include "WindowsUtil.h"

#include "../VeritasEngineBase/ResourceHandle.h"

#include "DDSTextureLoader/DDSTextureLoader.h"

#include "ExternalDirectXVariables.h"
#include "DirectXTextureData.h"

void VeritasEngine::TextureResourceLoaderImpl::Load(std::istream& data, ResourceHandle& handle) const
{
	data.seekg(0, std::ios::end);

	size_t length = static_cast<size_t>(data.tellg());

	data.seekg(0, std::ios::beg);

	uint8_t* img = new uint8_t[length];

	data.read(reinterpret_cast<char*>(img), length);

	ComPtr<ID3D11Resource> texture;
	ComPtr<ID3D11ShaderResourceView> textureView;
	HHR(DirectX::CreateDDSTextureFromMemory(g_device.Get(), g_context.Get(), img, length, texture.GetAddressOf(), textureView.GetAddressOf()), "Failed to load texture");

	delete[] img;

	handle.SetData(DirectXTextureData{ texture, textureView });
}
