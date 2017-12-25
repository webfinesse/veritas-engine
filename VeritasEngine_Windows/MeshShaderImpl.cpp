#include "MeshShaderImpl.h"

#include <d3d11.h>
#include <wrl\client.h>

#include "WindowsUtil.h"

#include "../VeritasEngine/PassBuffer.h"
#include "../VeritasEngine/FrameDescription.h"
#include "../VeritasEngine/PerObjectBuffer.h"
#include "DirectXTextureData.h"
#include "DirectXState.h"

#include "MeshVertxShader.h"
#include "MeshPixelShader.h"
#include "../VeritasEngine/IResourceManager.h"
#include "../VeritasEngine/ResourceData.h"

using namespace Microsoft::WRL;

struct VeritasEngine::MeshShaderImpl::Impl
{
	Impl(std::shared_ptr<DirectXState>&& dxState)
		: m_dxState { std::move(dxState) }
	{
		
	}

	void Init(std::shared_ptr<IResourceManager>&& resourceManager)
	{
		m_resourceManager = std::move(resourceManager);

		HHR(m_dxState->Device->CreateVertexShader(g_meshVertexShader, sizeof(g_meshVertexShader), nullptr, m_vertexShader.GetAddressOf()), "Failed creating vertex shader");

		D3D11_INPUT_ELEMENT_DESC ied[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "BINORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};

		HHR(m_dxState->Device->CreateInputLayout(ied, sizeof(ied) / sizeof(ied[0]), g_meshVertexShader, sizeof(g_meshVertexShader), m_inputLayout.GetAddressOf()), "Could not create input layout");

		HHR(m_dxState->Device->CreatePixelShader(g_meshPixelShader, sizeof(g_meshPixelShader), nullptr, m_pixelShader.GetAddressOf()), "failed creating pixel shader");

		
		D3D11_BUFFER_DESC cbDesc;
		cbDesc.ByteWidth = sizeof(PassBuffer);
		cbDesc.Usage = D3D11_USAGE_DYNAMIC;
		cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		cbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		cbDesc.MiscFlags = 0;
		cbDesc.StructureByteStride = 0;

		PassBuffer passBuffer;

		// Fill in the subresource data.
		D3D11_SUBRESOURCE_DATA initData;
		initData.pSysMem = &passBuffer;
		initData.SysMemPitch = 0;
		initData.SysMemSlicePitch = 0;

		HHR(m_dxState->Device->CreateBuffer(&cbDesc, &initData, m_cameraBuffer.GetAddressOf()), "failed creating camera buffer");

		PerObjectBuffer poBuffer;
		cbDesc.ByteWidth = sizeof(PerObjectBuffer);
		initData.pSysMem = &poBuffer;

		HHR(m_dxState->Device->CreateBuffer(&cbDesc, &initData, m_perObjectBuffer.GetAddressOf()), "failed creating perObject buffer");
	}

	void Activate()
	{
		m_dxState->Context->VSSetShader(m_vertexShader.Get(), nullptr, 0);
		m_dxState->Context->IASetInputLayout(m_inputLayout.Get());
		m_dxState->Context->PSSetShader(m_pixelShader.Get(), nullptr, 0);

		ID3D11Buffer* buffers[2] = { m_cameraBuffer.Get(), m_perObjectBuffer.Get() };

		m_dxState->Context->VSSetConstantBuffers(0, 2, buffers);
		m_dxState->Context->PSSetConstantBuffers(0, 2, buffers);
	}

	void Deactivate()
	{
		m_dxState->Context->VSSetShader(nullptr, nullptr, 0);
		m_dxState->Context->IASetInputLayout(nullptr);
		m_dxState->Context->PSSetShader(nullptr, nullptr, 0);
		m_dxState->Context->VSSetConstantBuffers(1, 0, nullptr);
		m_dxState->Context->PSSetConstantBuffers(1, 0, nullptr);
		m_dxState->Context->PSSetShaderResources(1, 0, nullptr);
	}

	void SetPassParameters(PassBuffer& passBuffer)
	{
		D3D11_MAPPED_SUBRESOURCE mappedResource;
		HHR(m_dxState->Context->Map(m_cameraBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource), "mapping camera");

		PassBuffer* dataPtr = static_cast<PassBuffer*>(mappedResource.pData);

		std::memcpy(dataPtr->Lights, passBuffer.Lights, sizeof(decltype(passBuffer.Lights)));
		WriteMatrixToBuffer(&dataPtr->ViewMatrix, passBuffer.ViewMatrix);
		WriteMatrixToBuffer(&dataPtr->ProjectionMatrix, passBuffer.ProjectionMatrix);
		dataPtr->EyePosition = passBuffer.EyePosition;	

		m_dxState->Context->Unmap(m_cameraBuffer.Get(), 0);
	}

	void SetPerObjectBuffer(const PerObjectBufferDescription& buffer)
	{
		D3D11_MAPPED_SUBRESOURCE mappedResource;
		HHR(m_dxState->Context->Map(m_perObjectBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource), "mapping per object");

		PerObjectBuffer* dataPtr = static_cast<PerObjectBuffer*>(mappedResource.pData);
		dataPtr->ShaderFlags = ShaderFlags_None;

		ID3D11ShaderResourceView* resources[4] = { nullptr, nullptr, nullptr, nullptr };

		m_resourceManager->GetResource(buffer.Material, [&](const ResourceData& m)
		{
			const auto& materialInstance = m.GetData<MaterialInstance>();

			std::memcpy(&dataPtr->Material, &materialInstance.Material, sizeof(GraphicsCardMaterial));

			const auto hasDiffuseMap = materialInstance.DiffuseMap != 0;
			dataPtr->ShaderFlags |= hasDiffuseMap ? ShaderFlags_HasDiffuseMap : ShaderFlags_None;

			if (hasDiffuseMap)
			{
				m_resourceManager->GetResource(materialInstance.DiffuseMap, [&](const ResourceData& texture)
				{
					resources[0] = texture.GetData<DirectXTextureData>().TextureView.Get();
				});
			}

			const auto hasNormalMap = materialInstance.NormalMap != 0;
			dataPtr->ShaderFlags |= hasNormalMap ? ShaderFlags_HasNormalMap : ShaderFlags_None;

			if (hasNormalMap)
			{
				m_resourceManager->GetResource(materialInstance.NormalMap, [&](const ResourceData& texture)
				{
					resources[1] = texture.GetData<DirectXTextureData>().TextureView.Get();
				});
			}

			const auto hasSpecularMap = materialInstance.SpecularMap != 0;
			dataPtr->ShaderFlags |= hasSpecularMap ? ShaderFlags_HasSpecularMap : ShaderFlags_None;

			if (hasSpecularMap)
			{
				m_resourceManager->GetResource(materialInstance.SpecularMap, [&](const ResourceData& texture)
				{
					resources[2] = texture.GetData<DirectXTextureData>().TextureView.Get();
				});
			}

			const auto hasTransparencyMap = materialInstance.TransparentMap != 0;
			dataPtr->ShaderFlags |= hasTransparencyMap ? ShaderFlags_HasTransparancyMap : ShaderFlags_None;

			if (hasTransparencyMap)
			{
				m_resourceManager->GetResource(materialInstance.TransparentMap, [&](const ResourceData& texture)
				{
					resources[3] = texture.GetData<DirectXTextureData>().TextureView.Get();
				});
			}
		});

		WriteMatrixToBuffer(&dataPtr->WorldTransform, buffer.WorldTransform);
		WriteMatrixToBuffer(&dataPtr->WorldInverseTranspose, buffer.WorldInverseTranspose);

		m_dxState->Context->Unmap(m_perObjectBuffer.Get(), 0);

		m_dxState->Context->PSSetShaderResources(0, 4, resources);
	}

	static void WriteMatrixToBuffer(Matrix4x4* destination, const Matrix4x4& matrixToTranspose)
	{
		*destination = matrixToTranspose;
	}

	ComPtr<ID3D11Buffer> m_cameraBuffer;
	ComPtr<ID3D11Buffer> m_perObjectBuffer;
	ComPtr<ID3D11InputLayout> m_inputLayout;
	ComPtr<ID3D11VertexShader> m_vertexShader;
	ComPtr<ID3D11PixelShader> m_pixelShader;
	std::shared_ptr<DirectXState> m_dxState;
	std::shared_ptr<IResourceManager> m_resourceManager;
};

VeritasEngine::MeshShaderImpl::MeshShaderImpl(std::shared_ptr<DirectXState> dxState)
	: m_impl(std::make_unique<Impl>(std::move(dxState)))
{

}

VeritasEngine::MeshShaderImpl::MeshShaderImpl(MeshShaderImpl&& other) noexcept
	: m_impl{ std::move(other.m_impl) }
{

}

VeritasEngine::MeshShaderImpl::~MeshShaderImpl() noexcept = default;

VeritasEngine::MeshShaderImpl& VeritasEngine::MeshShaderImpl::operator=(MeshShaderImpl&& other) noexcept
{
	if(this != &other)
	{
		m_impl = std::move(other.m_impl);
	}

	return *this;
}

void VeritasEngine::MeshShaderImpl::Init(std::shared_ptr<IResourceManager> resourceManager)
{
	m_impl->Init(std::move(resourceManager));
}

void VeritasEngine::MeshShaderImpl::Activate()
{
	m_impl->Activate();
}

void VeritasEngine::MeshShaderImpl::Deactivate()
{
	m_impl->Deactivate();
}

void VeritasEngine::MeshShaderImpl::SetPassParameters(PassBuffer& passBuffer)
{
	m_impl->SetPassParameters(passBuffer);
}

void VeritasEngine::MeshShaderImpl::SetPerObjectBuffer(const PerObjectBufferDescription& buffer)
{
	m_impl->SetPerObjectBuffer(buffer);
}