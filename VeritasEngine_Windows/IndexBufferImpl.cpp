#include <d3d11.h>
#include <wrl\client.h>
#include <vector>

using namespace Microsoft::WRL;

#include "IndexBufferImpl.h"
#include "../VeritasEngine/BufferIndicies.h"
#include "WindowsUtil.h"

extern ComPtr<ID3D11Device> g_device;

struct VeritasEngine::IndexBufferImpl::Impl
{
	Impl()
		: m_buffer{}, m_indicies{}
	{

	}

	void SetBufferData()
	{
		D3D11_BUFFER_DESC bufferDesc;
		bufferDesc.Usage = D3D11_USAGE_DEFAULT;
		bufferDesc.ByteWidth = static_cast<UINT>(sizeof(unsigned int) * m_indicies.size());
		bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bufferDesc.CPUAccessFlags = 0;
		bufferDesc.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA initData;
		initData.pSysMem = &m_indicies[0];
		initData.SysMemPitch = 0;
		initData.SysMemSlicePitch = 0;

		ComPtr<ID3D11Buffer> newBuffer;
		HHR(g_device->CreateBuffer(&bufferDesc, &initData, &newBuffer), "Error Creating Index Buffer");

		m_buffer = newBuffer;
	}

	BufferIndicies AddIndicies(unsigned int* indicies, size_t numOfIndicies)
	{
		BufferIndicies result(m_indicies.size(), numOfIndicies);

		auto end = m_indicies.end();
		m_indicies.insert(end, indicies, indicies + numOfIndicies);

		SetBufferData();

		return result;
	}

	ComPtr<ID3D11Buffer> m_buffer;
	vector<unsigned int> m_indicies;
};

VeritasEngine::IndexBufferImpl::IndexBufferImpl()
	: m_impl(std::make_unique<Impl>())
{

}

VeritasEngine::IndexBufferImpl::~IndexBufferImpl() = default;

VeritasEngine::BufferIndicies VeritasEngine::IndexBufferImpl::AddIndicies(unsigned int* indicies, size_t numOfIndicies)
{
	return m_impl->AddIndicies(indicies, numOfIndicies);
}

void* VeritasEngine::IndexBufferImpl::GetNativeBuffer() const
{
	return m_impl->m_buffer.Get();
}