#include "MeshShaderImpl.h"

#include <d3d11.h>
#include <wrl\client.h>

#include "WindowsUtil.h"

#include "../VeritasEngine/PassBuffer.h"
#include "../VeritasEngine/FrameDescription.h"
#include "../VeritasEngine/PerObjectBuffer.h"
#include "../VeritasEngineBase/ResourceHandle.h"
#include "DirectXTextureData.h"
#include "DirectXState.h"

#include "MeshVertxShader.h"
#include "MeshPixelShader.h"

using namespace Microsoft::WRL;

struct VeritasEngine::MeshShaderImpl::Impl
{
	Impl(std::shared_ptr<DirectXState> dxState)
		: m_dxState { dxState }
	{
		
	}

	void Init()
	{
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

		auto hasDiffuseMap = buffer.Material->DiffuseMap != nullptr;
		dataPtr->HasDiffuseMap = hasDiffuseMap ? 1 : 0;

		auto hasNormalMap = buffer.Material->NormalMap != nullptr;
		dataPtr->HasNormalMap = hasNormalMap ? 1 : 0;

		auto hasSpecularMap = buffer.Material->SpecularMap != nullptr;
		dataPtr->HasSpecularMap = hasSpecularMap ? 1 : 0;

		WriteMatrixToBuffer(&dataPtr->WorldTransform, buffer.WorldTransform);
		WriteMatrixToBuffer(&dataPtr->WorldInverseTranspose, buffer.WorldInverseTranspose);

		std::memcpy(&dataPtr->Material, &buffer.Material->Material, sizeof(GraphicsCardMaterial));

		m_dxState->Context->Unmap(m_perObjectBuffer.Get(), 0);

		ID3D11ShaderResourceView* resources[3] = { nullptr, nullptr, nullptr };

		if (hasDiffuseMap)
		{
			const DirectXTextureData& textureData = buffer.Material->DiffuseMap->GetData<DirectXTextureData>();

			resources[0] = textureData.TextureView.Get();
		}

		if (hasNormalMap)
		{
			const DirectXTextureData& textureData = buffer.Material->NormalMap->GetData<DirectXTextureData>();

			resources[1] = textureData.TextureView.Get();
		}

		if (hasSpecularMap)
		{
			const DirectXTextureData& textureData = buffer.Material->SpecularMap->GetData<DirectXTextureData>();

			resources[2] = textureData.TextureView.Get();
		}

		m_dxState->Context->PSSetShaderResources(0, 3, resources);
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
};

VeritasEngine::MeshShaderImpl::MeshShaderImpl(std::shared_ptr<DirectXState> dxState)
	: m_impl(std::make_unique<Impl>(dxState))
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

void VeritasEngine::MeshShaderImpl::Init()
{
	m_impl->Init();
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