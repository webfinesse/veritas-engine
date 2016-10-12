#include "VertexBuffer.h"
#include "BufferIndicies.h"
#include "../VeritasEngine_Windows/WindowsUtil.h"
#include "../VeritasEngineBase/VertexBufferImpl.h"

#include <vector>

struct VeritasEngine::VertexBuffer::Impl : public VeritasEngine::SmallObject<>
{
	Impl(size_t sizeOfVertex)
		: m_buffer{}, m_sizeOfVertex{ sizeOfVertex }, m_numVerticies{ 0 }
	{
		
	}

	void SetBufferData()
	{
		m_buffer.SetData(m_numVerticies, m_sizeOfVertex, &m_verticies[0]);
	}

	VertexBufferImpl m_buffer;
	vector<gsl::byte> m_verticies;
	size_t m_sizeOfVertex;
	size_t m_numVerticies;
};

VeritasEngine::VertexBuffer::VertexBuffer()
	: VertexBuffer(0)
{

}

VeritasEngine::VertexBuffer::VertexBuffer(size_t sizeOfVertex)
	: m_impl{ std::make_unique<Impl>(sizeOfVertex) }
{

}

VeritasEngine::VertexBuffer::~VertexBuffer() = default;

VeritasEngine::BufferIndicies VeritasEngine::VertexBuffer::AddVerticies(gsl::byte* verticies, size_t numVerticies)
{
	auto bytesPerVertex = m_impl->m_sizeOfVertex / sizeof(gsl::byte);

	auto end = m_impl->m_verticies.end();
	m_impl->m_verticies.insert(end, verticies, verticies + (bytesPerVertex * numVerticies));

	BufferIndicies result(m_impl->m_numVerticies, numVerticies);

	m_impl->m_numVerticies += numVerticies;

	m_impl->SetBufferData();

	return result;
}

size_t VeritasEngine::VertexBuffer::GetVertexSize() const
{
	return m_impl->m_sizeOfVertex;
}

void* VeritasEngine::VertexBuffer::GetNativeBuffer()
{
	return m_impl->m_buffer.GetNativeBuffer();
}