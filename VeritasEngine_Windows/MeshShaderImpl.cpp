#include "../VeritasEngineBase/MeshShaderImpl.h"

#include <d3d11.h>
#include <wrl\client.h>

#include "ExternalDirectXVariables.h"
#include "WindowsUtil.h"

#include "../VeritasEngineBase/CameraBuffer.h"
#include "../VeritasEngineBase/PerFrameBuffer.h"
#include "../VeritasEngineBase/PerObjectBuffer.h"
#include "../VeritasEngineBase/ResourceHandle.h"
#include "../VeritasEngineBase/FileHelper.h"
#include "DirectXTextureData.h"

using namespace Microsoft::WRL;

struct VeritasEngine::MeshShaderImpl::Impl
{
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
};

VeritasEngine::MeshShaderImpl::MeshShaderImpl()
	: m_impl(std::make_unique<Impl>())
{

}

VeritasEngine::MeshShaderImpl::~MeshShaderImpl() = default;

void VeritasEngine::MeshShaderImpl::Init()
{
	unsigned int length;
	auto* fileData = FileHelper::ReadFile("VertexShader.cso", &length);

	HHR(g_device->CreateVertexShader(fileData, length, nullptr, &m_impl->m_vertexShader), "Failed creating vertex shader");

	D3D11_INPUT_ELEMENT_DESC ied[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "BINORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 36, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 48, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	HHR(g_device->CreateInputLayout(ied, sizeof(ied) / sizeof(ied[0]), fileData, length, &m_impl->m_inputLayout), "Could not create input layout");

	delete[] fileData;
	fileData = nullptr;

	fileData = FileHelper::ReadFile("PixelShader.cso", &length);

	HHR(g_device->CreatePixelShader(fileData, length, nullptr, &m_impl->m_pixelShader), "failed creating pixel shader");

	delete[] fileData;

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

	HHR(g_device->CreateBuffer(&cbDesc, &initData, m_impl->m_cameraBuffer.GetAddressOf()), "failed creating camera buffer");

	PerObjectBuffer poBuffer;
	cbDesc.ByteWidth = sizeof(PerObjectBuffer);
	initData.pSysMem = &poBuffer;
	
	HHR(g_device->CreateBuffer(&cbDesc, &initData, m_impl->m_perObjectBuffer.GetAddressOf()), "failed creating perObject buffer");

	PerFrameBuffer perFrameBuffer;
	cbDesc.ByteWidth = sizeof(PerFrameBuffer);
	initData.pSysMem = &perFrameBuffer;

	HHR(g_device->CreateBuffer(&cbDesc, &initData, m_impl->m_perFrame.GetAddressOf()), "failed creating perFrame buffer");
}

void VeritasEngine::MeshShaderImpl::Activate()
{
	g_context->VSSetShader(m_impl->m_vertexShader.Get(), nullptr, 0);
	g_context->IASetInputLayout(m_impl->m_inputLayout.Get());
	g_context->PSSetShader(m_impl->m_pixelShader.Get(), nullptr, 0);

	ID3D11Buffer* buffers[3] = { m_impl->m_cameraBuffer.Get(), m_impl->m_perObjectBuffer.Get(), m_impl->m_perFrame.Get() };

	g_context->VSSetConstantBuffers(0, 3, buffers);
	g_context->PSSetConstantBuffers(0, 3, buffers);
}

void VeritasEngine::MeshShaderImpl::Deactivate()
{
	g_context->VSSetShader(nullptr, nullptr, 0);
	g_context->IASetInputLayout(nullptr);
	g_context->PSSetShader(nullptr, nullptr, 0);

	g_context->VSSetConstantBuffers(1, 0, nullptr);
	g_context->PSSetConstantBuffers(1, 0, nullptr);
	g_context->PSSetShaderResources(1, 0, nullptr);
}

void VeritasEngine::MeshShaderImpl::SetLightParameters(const std::array<VeritasEngine::Light, Light::MAX_LIGHTS>& lightsArray)
{
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	HHR(g_context->Map(m_impl->m_perFrame.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource), "mapping per frame");

	PerFrameBuffer* dataPtr = static_cast<PerFrameBuffer*>(mappedResource.pData);

	memcpy(dataPtr->lights, &lightsArray[0], Light::MAX_LIGHTS * sizeof(Light));

	g_context->Unmap(m_impl->m_perFrame.Get(), 0);
}

void VeritasEngine::MeshShaderImpl::SetCameraParameters(const Float3 & eyePosition, const Matrix4x4 & viewMatrix, const Matrix4x4 & projectionMatrix)
{
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	HHR(g_context->Map(m_impl->m_cameraBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource), "mapping camera");

	CameraBuffer* dataPtr = static_cast<CameraBuffer*>(mappedResource.pData);

	MeshShaderImpl::Impl::TransposeForBuffer(&dataPtr->m_viewMatrix, viewMatrix);
	MeshShaderImpl::Impl::TransposeForBuffer(&dataPtr->m_projectionMatrix, projectionMatrix);
	dataPtr->m_cameraPosition = eyePosition;

	g_context->Unmap(m_impl->m_cameraBuffer.Get(), 0);
}

void VeritasEngine::MeshShaderImpl::SetPerObjectBuffer(const PerObjectBufferRef& buffer)
{
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	HHR(g_context->Map(m_impl->m_perObjectBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource), "mapping per object");

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

	g_context->Unmap(m_impl->m_perObjectBuffer.Get(), 0);

	ID3D11ShaderResourceView* resources[3] = { nullptr, nullptr, nullptr };

	if(hasDiffuseMap)
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

	g_context->PSSetShaderResources(0, 3, resources);
}