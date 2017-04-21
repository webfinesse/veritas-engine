#include <d3d11.h>
#include <wrl\client.h>

using namespace Microsoft::WRL;

#include "../VeritasEngineBase/IndexBufferImpl.h"
#include "WindowsUtil.h"

extern ComPtr<ID3D11Device> g_device;

struct VeritasEngine::IndexBufferImpl::Impl
{
	Impl()
		: m_buffer{}
	{

	}

	ComPtr<ID3D11Buffer> m_buffer;
};

VeritasEngine::IndexBufferImpl::IndexBufferImpl()
	: m_impl(std::make_unique<Impl>())
{

}

VeritasEngine::IndexBufferImpl::~IndexBufferImpl() = default;

void* VeritasEngine::IndexBufferImpl::GetNativeBuffer() const
{
	return m_impl->m_buffer.Get();
}


void VeritasEngine::IndexBufferImpl::SetData(size_t numOfElements, void* bufferData)
{
	D3D11_BUFFER_DESC bufferDesc;
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = static_cast<UINT>(sizeof(unsigned int) * numOfElements);
	bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA initData;
	initData.pSysMem = bufferData;
	initData.SysMemPitch = 0;
	initData.SysMemSlicePitch = 0;

	ComPtr<ID3D11Buffer> newBuffer;
	HHR(g_device->CreateBuffer(&bufferDesc, &initData, &newBuffer), "Error Creating Index Buffer");

	m_impl->m_buffer = newBuffer;
}