#include <d3d11.h>
#include <wrl\client.h>
#include <vector>
#include "DirectXState.h"

using namespace Microsoft::WRL;

#include "IndexBufferImpl.h"
#include "../VeritasEngine/BufferIndicies.h"
#include "WindowsUtil.h"

struct VeritasEngine::IndexBufferImpl::Impl
{
	Impl(std::shared_ptr<DirectXState> dxState)
		: m_buffer{}, m_dxState{ dxState }, m_indicies{}
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
		HHR(m_dxState->Device->CreateBuffer(&bufferDesc, &initData, newBuffer.GetAddressOf()), "Error Creating Index Buffer");

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
	std::shared_ptr<DirectXState> m_dxState;
	std::vector<unsigned int> m_indicies;
};

VeritasEngine::IndexBufferImpl::IndexBufferImpl(std::shared_ptr<DirectXState> dxState)
	: m_impl(std::make_unique<Impl>(dxState))
{

}

VeritasEngine::IndexBufferImpl::IndexBufferImpl(IndexBufferImpl&& other) noexcept
	: m_impl { std::move(other.m_impl) }
{
}

VeritasEngine::IndexBufferImpl::~IndexBufferImpl() = default;

VeritasEngine::IndexBufferImpl& VeritasEngine::IndexBufferImpl::operator=(IndexBufferImpl&& other) noexcept
{
	if(this != &other)
	{
		m_impl = std::move(other.m_impl);
	}

	return *this;
}

VeritasEngine::BufferIndicies VeritasEngine::IndexBufferImpl::AddIndicies(unsigned int* indicies, size_t numOfIndicies)
{
	return m_impl->AddIndicies(indicies, numOfIndicies);
}

void* VeritasEngine::IndexBufferImpl::GetNativeBuffer() const
{
	return m_impl->m_buffer.Get();
}