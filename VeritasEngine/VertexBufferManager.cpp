#include "VertexBufferManager.h"
#include "VertexBuffer.h"
#include "VertexTypeHandle.h"
#include "../Includes/AssocVector/AssocVector.hpp"

struct VeritasEngine::VertexBufferManager::Impl: public VeritasEngine::SmallObject<>
{
	Impl()
		: m_buffers{}
	{

	}

	AssocVector<VertexTypeHandle, std::shared_ptr<VertexBuffer>> m_buffers;
};

void VeritasEngine::VertexBufferManager::RegisterVertexFormat(VertexTypeHandle handle, size_t sizeOfVertex)
{
	auto buffer = std::make_shared<VertexBuffer>(sizeOfVertex);

	m_impl->m_buffers[handle] = buffer;
}

std::shared_ptr<VeritasEngine::VertexBuffer> VeritasEngine::VertexBufferManager::GetBuffer(VertexTypeHandle handle) const
{
	auto find = m_impl->m_buffers.find(handle);

	if (find != m_impl->m_buffers.end())
	{
		return find->second;
	}
	else
	{
		return nullptr;
	}
}

VeritasEngine::VertexBufferManager::VertexBufferManager()
	: m_impl(std::make_unique<Impl>())
{
	
}

VeritasEngine::VertexBufferManager::~VertexBufferManager() = default;
