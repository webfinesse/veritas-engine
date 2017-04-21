#include "IndexBuffer.h"
#include "BufferIndicies.h"
#include "../VeritasEngineBase/IndexBufferImpl.h"

#include <vector>

struct VeritasEngine::IndexBuffer::Impl : public VeritasEngine::SmallObject<>
{
	Impl()
		: m_buffer{}
	{

	}

	void SetBufferData()
	{
		m_buffer.SetData(m_indicies.size(), &m_indicies[0]);
	}

	IndexBufferImpl m_buffer;
	std::vector<unsigned int> m_indicies;
};

VeritasEngine::IndexBuffer::IndexBuffer()
	: m_impl(std::make_unique<Impl>())
{

}

VeritasEngine::IndexBuffer::~IndexBuffer() = default;

VeritasEngine::BufferIndicies VeritasEngine::IndexBuffer::AddIndicies(unsigned int* indicies, size_t numOfIndicies)
{
	BufferIndicies result(m_impl->m_indicies.size(), numOfIndicies);

	auto end = m_impl->m_indicies.end();
	m_impl->m_indicies.insert(end, indicies, indicies + numOfIndicies);

	m_impl->SetBufferData();

	return result;
}

void* VeritasEngine::IndexBuffer::GetNativeBuffer()
{
	return m_impl->m_buffer.GetNativeBuffer();
}