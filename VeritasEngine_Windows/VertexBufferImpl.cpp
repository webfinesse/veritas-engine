#include "../VeritasEngineBase/VertexBufferImpl.h"
#include "WindowsUtil.h"

#include <d3d11.h>
#include <wrl\client.h>

using namespace Microsoft::WRL;

extern ComPtr<ID3D11Device> g_device;
extern ComPtr<ID3D11DeviceContext> g_context;

struct VeritasEngine::VertexBufferImpl::Impl
{
	Impl()
		: m_buffer{}
	{

	}

	ComPtr<ID3D11Buffer> m_buffer;
};

VeritasEngine::VertexBufferImpl::VertexBufferImpl()
	: m_impl(std::make_unique<Impl>())
{

}

VeritasEngine::VertexBufferImpl::~VertexBufferImpl() = default;

void VeritasEngine::VertexBufferImpl::SetData(unsigned int numOfElements, unsigned int sizeOfEachVertex, void* bufferData)
{
	D3D11_BUFFER_DESC bufferDesc;
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = sizeOfEachVertex * numOfElements;
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA initData;
	initData.pSysMem = bufferData;
	initData.SysMemPitch = 0;
	initData.SysMemSlicePitch = 0;

	ComPtr<ID3D11Buffer> newBuffer;

	HHR(g_device->CreateBuffer(&bufferDesc, &initData, &newBuffer), "Error creating Vertex Buffer");

	m_impl->m_buffer = newBuffer;
}

void* VeritasEngine::VertexBufferImpl::GetNativeBuffer() const
{
	return m_impl->m_buffer.Get();
}