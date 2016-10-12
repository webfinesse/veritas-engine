#include "IndexBufferManager.h"
#include "IndexBuffer.h"

struct VeritasEngine::IndexBufferManager::Impl : public VeritasEngine::SmallObject<>
{
	Impl()
		: m_buffer{ std::make_unique<IndexBuffer>() }
	{

	}

	std::unique_ptr<IndexBuffer> m_buffer;
};


VeritasEngine::IndexBuffer& VeritasEngine::IndexBufferManager::GetBuffer()
{
	return *m_impl->m_buffer;
}

VeritasEngine::IndexBufferManager::IndexBufferManager()
	: m_impl{ std::make_unique<Impl>() }
{

}

VeritasEngine::IndexBufferManager::~IndexBufferManager() = default;