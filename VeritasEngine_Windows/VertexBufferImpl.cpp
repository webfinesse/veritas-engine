#include "VertexBufferImpl.h"
#include "DirectXState.h"
#include "../VeritasEngine/BufferIndicies.h"
#include "WindowsUtil.h"

#include <d3d11.h>
#include <wrl\client.h>
#include <vector>

using namespace Microsoft::WRL;

struct VeritasEngine::VertexBufferImpl::Impl
{
	Impl(const std::shared_ptr<DirectXState>&& dxState, const size_t vertexSize)
		: m_dxState{ std::move(dxState) }, m_sizeOfVertex{ vertexSize }
	{

	}

	void SetData()
	{
		D3D11_BUFFER_DESC bufferDesc;
		bufferDesc.Usage = D3D11_USAGE_DEFAULT;
		bufferDesc.ByteWidth = static_cast<UINT>(m_sizeOfVertex * m_numVerticies);
		bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bufferDesc.CPUAccessFlags = 0;
		bufferDesc.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA initData;
		initData.pSysMem = &m_verticies[0];
		initData.SysMemPitch = 0;
		initData.SysMemSlicePitch = 0;

		ComPtr<ID3D11Buffer> newBuffer;

		HHR(m_dxState->Device->CreateBuffer(&bufferDesc, &initData, newBuffer.GetAddressOf()), "Error creating Vertex Buffer");

		m_buffer = newBuffer;
	}

	BufferIndicies AddVerticies(unsigned char* verticies, const size_t numVerticies)
	{
		const auto bytesPerVertex = m_sizeOfVertex / sizeof(unsigned char);

		const auto end = m_verticies.end();
		m_verticies.insert(end, verticies, verticies + (bytesPerVertex * numVerticies));

		const BufferIndicies result{ m_numVerticies, numVerticies };

		m_numVerticies += numVerticies;

		SetData();

		return result;
	}

	ComPtr<ID3D11Buffer> m_buffer{nullptr};
	std::shared_ptr<DirectXState> m_dxState;
	std::vector<unsigned char> m_verticies{};
	size_t m_sizeOfVertex;
	size_t m_numVerticies{0};
};

VeritasEngine::VertexBufferImpl::VertexBufferImpl(std::shared_ptr<DirectXState> dxState, const size_t vertexSize)
	: m_impl(std::make_unique<Impl>(std::move(dxState), vertexSize))
{

}

VeritasEngine::VertexBufferImpl::~VertexBufferImpl() = default;

VeritasEngine::BufferIndicies VeritasEngine::VertexBufferImpl::AddVerticies(unsigned char* verticies, const size_t numVerticies)
{
	return m_impl->AddVerticies(verticies, numVerticies);
}

size_t VeritasEngine::VertexBufferImpl::GetVertexSize() const
{
	return m_impl->m_sizeOfVertex;
}

void* VeritasEngine::VertexBufferImpl::GetNativeBuffer() const
{
	return m_impl->m_buffer.Get();
}