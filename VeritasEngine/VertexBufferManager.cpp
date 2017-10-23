#include "VertexBufferManager.h"
#include "IVertexBuffer.h"
#include "VertexTypeHandle.h"
#include "IVertexBufferFactory.h"
#include "../Includes/AssocVector/AssocVector.hpp"

struct VeritasEngine::VertexBufferManager::Impl: public VeritasEngine::SmallObject<>
{
	Impl(std::shared_ptr<IVertexBufferFactory> factory)
		: m_factory{ factory }
	{

	}

	std::shared_ptr<IVertexBufferFactory> m_factory;
	AssocVector<VertexTypeHandle, std::shared_ptr<IVertexBuffer>> m_buffers{};
};

void VeritasEngine::VertexBufferManager::RegisterVertexFormat(VertexTypeHandle handle, size_t sizeOfVertex)
{
	const auto buffer = m_impl->m_factory->Create(sizeOfVertex);

	m_impl->m_buffers[handle] = buffer;
}

VeritasEngine::IVertexBuffer* VeritasEngine::VertexBufferManager::GetBuffer(VertexTypeHandle handle) const
{
	auto find = m_impl->m_buffers.find(handle);

	if (find != m_impl->m_buffers.end())
	{
		return find->second.get();
	}
	else
	{
		return nullptr;
	}
}

VeritasEngine::VertexBufferManager::VertexBufferManager(std::shared_ptr<IVertexBufferFactory> factory)
	: m_impl(std::make_unique<Impl>(factory))
{
	
}

VeritasEngine::VertexBufferManager::VertexBufferManager(VertexBufferManager&& other) noexcept
	: m_impl{ std::move(other.m_impl) }
{
	
}

VeritasEngine::VertexBufferManager::~VertexBufferManager() = default;

VeritasEngine::VertexBufferManager& VeritasEngine::VertexBufferManager::operator=(VertexBufferManager&& other) noexcept
{
	if(this != &other)
	{
		m_impl = std::move(other.m_impl);
	}

	return *this;
}
