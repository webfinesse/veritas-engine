#include "MeshShaderImpl.h"

#include <d3d11.h>
#include <wrl\client.h>

#include "WindowsUtil.h"

#include "../VeritasEngineBase/CameraBuffer.h"
#include "../VeritasEngineBase/PerFrameBuffer.h"
#include "../VeritasEngineBase/PerObjectBuffer.h"
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
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "BINORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 36, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 48, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};

		HHR(m_dxState->Device->CreateInputLayout(ied, sizeof(ied) / sizeof(ied[0]), g_meshVertexShader, sizeof(g_meshVertexShader), m_inputLayout.GetAddressOf()), "Could not create input layout");

		HHR(m_dxState->Device->CreatePixelShader(g_meshPixelShader, sizeof(g_meshPixelShader), nullptr, m_pixelShader.GetAddressOf()), "failed creating pixel shader");

		
		D3D11_BUFFER_DESC cbDesc;
		cbDesc.ByteWidth = sizeof(CameraBuffer);
		cbDesc.Usage = D3D11_USAGE_DYNAMIC;
		cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		cbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		cbDesc.MiscFlags = 0;
		cbDesc.StructureByteStride = 0;

		CameraBuffer cameraBuffer;

		// Fill in the subresource data.
		D3D11_SUBRESOURCE_DATA initData;
		initData.pSysMem = &cameraBuffer;
		initData.SysMemPitch = 0;
		initData.SysMemSlicePitch = 0;

		HHR(m_dxState->Device->CreateBuffer(&cbDesc, &initData, m_cameraBuffer.GetAddressOf()), "failed creating camera buffer");

		PerObjectBuffer poBuffer;
		cbDesc.ByteWidth = sizeof(PerObjectBuffer);
		initData.pSysMem = &poBuffer;

		HHR(m_dxState->Device->CreateBuffer(&cbDesc, &initData, m_perObjectBuffer.GetAddressOf()), "failed creating perObject buffer");

		PerFrameBuffer perFrameBuffer;
		cbDesc.ByteWidth = sizeof(PerFrameBuffer);
		initData.pSysMem = &perFrameBuffer;

		HHR(m_dxState->Device->CreateBuffer(&cbDesc, &initData, m_perFrame.GetAddressOf()), "failed creating perFrame buffer");
	}

	void Activate()
	{
		m_dxState->Context->VSSetShader(m_vertexShader.Get(), nullptr, 0);
		m_dxState->Context->IASetInputLayout(m_inputLayout.Get());
		m_dxState->Context->PSSetShader(m_pixelShader.Get(), nullptr, 0);

		ID3D11Buffer* buffers[3] = { m_cameraBuffer.Get(), m_perObjectBuffer.Get(), m_perFrame.Get() };

		m_dxState->Context->VSSetConstantBuffers(0, 3, buffers);
		m_dxState->Context->PSSetConstantBuffers(0, 3, buffers);
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

	void SetLightParameters(const std::array<VeritasEngine::Light, Light::MAX_LIGHTS>& lightsArray)
	{
		D3D11_MAPPED_SUBRESOURCE mappedResource;
		HHR(m_dxState->Context->Map(m_perFrame.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource), "mapping per frame");

		PerFrameBuffer* dataPtr = static_cast<PerFrameBuffer*>(mappedResource.pData);

		memcpy(dataPtr->lights, &lightsArray[0], Light::MAX_LIGHTS * sizeof(Light));

		m_dxState->Context->Unmap(m_perFrame.Get(), 0);
	}

	void SetCameraParameters(const Float3 & eyePosition, const Matrix4x4 & viewMatrix, const Matrix4x4 & projectionMatrix)
	{
		D3D11_MAPPED_SUBRESOURCE mappedResource;
		HHR(m_dxState->Context->Map(m_cameraBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource), "mapping camera");

		CameraBuffer* dataPtr = static_cast<CameraBuffer*>(mappedResource.pData);

		TransposeForBuffer(&dataPtr->m_viewMatrix, viewMatrix);
		TransposeForBuffer(&dataPtr->m_projectionMatrix, projectionMatrix);
		dataPtr->m_cameraPosition = eyePosition;

		m_dxState->Context->Unmap(m_cameraBuffer.Get(), 0);
	}

	void SetPerObjectBuffer(const PerObjectBufferRef& buffer)
	{
		D3D11_MAPPED_SUBRESOURCE mappedResource;
		HHR(m_dxState->Context->Map(m_perObjectBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource), "mapping per object");

		PerObjectBuffer* dataPtr = static_cast<PerObjectBuffer*>(mappedResource.pData);

		auto hasDiffuseMap = buffer.m_material.DiffuseMap != nullptr;
		dataPtr->hasDiffuseMap = buffer.m_material.DiffuseMap != nullptr ? 1 : 0;

		auto hasNormalMap = buffer.m_material.NormalMap != nullptr;
		dataPtr->hasNormalMap = buffer.m_material.NormalMap != nullptr ? 1 : 0;

		auto hasSpecularMap = buffer.m_material.SpecularMap != nullptr;
		dataPtr->hasSpecularMap = buffer.m_material.SpecularMap != nullptr ? 1 : 0;

		MeshShaderImpl::Impl::TransposeForBuffer(&dataPtr->m_worldTransform, buffer.m_worldTransform);
		MeshShaderImpl::Impl::TransposeForBuffer(&dataPtr->m_worldInverseTranspose, buffer.m_worldInverseTranspose);

		memcpy_s(&dataPtr->m_material, sizeof(GraphicsCardMaterial), &buffer.m_material, sizeof(GraphicsCardMaterial));

		m_dxState->Context->Unmap(m_perObjectBuffer.Get(), 0);

		ID3D11ShaderResourceView* resources[3] = { nullptr, nullptr, nullptr };

		if (hasDiffuseMap)
		{
			const DirectXTextureData& textureData = buffer.m_material.DiffuseMap->GetData<DirectXTextureData>();

			resources[0] = textureData.TextureView.Get();
		}

		if (hasNormalMap)
		{
			const DirectXTextureData& textureData = buffer.m_material.NormalMap->GetData<DirectXTextureData>();

			resources[1] = textureData.TextureView.Get();
		}

		if (hasSpecularMap)
		{
			const DirectXTextureData& textureData = buffer.m_material.SpecularMap->GetData<DirectXTextureData>();

			resources[2] = textureData.TextureView.Get();
		}

		m_dxState->Context->PSSetShaderResources(0, 3, resources);
	}

	static void TransposeForBuffer(Matrix4x4* destination, const Matrix4x4& matrixToTranspose)
	{
		auto transposed = MathHelpers::Transpose(matrixToTranspose);
		*destination = transposed;
	}

	ComPtr<ID3D11Buffer> m_cameraBuffer;
	ComPtr<ID3D11Buffer> m_perObjectBuffer;
	ComPtr<ID3D11Buffer> m_perFrame;
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

void VeritasEngine::MeshShaderImpl::SetLightParameters(const std::array<VeritasEngine::Light, Light::MAX_LIGHTS>& lightsArray)
{
	m_impl->SetLightParameters(lightsArray);
}

void VeritasEngine::MeshShaderImpl::SetCameraParameters(const Float3 & eyePosition, const Matrix4x4 & viewMatrix, const Matrix4x4 & projectionMatrix)
{
	m_impl->SetCameraParameters(eyePosition, viewMatrix, projectionMatrix);
}

void VeritasEngine::MeshShaderImpl::SetPerObjectBuffer(const PerObjectBufferRef& buffer)
{
	m_impl->SetPerObjectBuffer(buffer);
}