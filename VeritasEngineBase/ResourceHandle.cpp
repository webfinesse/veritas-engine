#include "ResourceHandle.h"

VeritasEngine::ResourceHandle::ResourceHandle()
	: m_data(std::make_unique<Impl>())
{
	
}

VeritasEngine::ResourceHandle::~ResourceHandle()
{
}

VeritasEngine::ResourceHandle::ResourceHandle(const ResourceHandle& rhs)
	: m_data(std::make_unique<Impl>(*rhs.m_data))
{

}

VeritasEngine::ResourceHandle& VeritasEngine::ResourceHandle::operator=(const VeritasEngine::ResourceHandle& rhs)
{
	if (this != &rhs)
	{
		*m_data = *rhs.m_data;
	}

	return *this;
}