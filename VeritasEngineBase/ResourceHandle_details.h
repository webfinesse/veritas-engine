#pragma once

#include <boost/any.hpp>

struct VeritasEngine::ResourceHandle::Impl
{
	boost::any m_any;
};

template <typename T>
const T& VeritasEngine::ResourceHandle::GetData() const
{
	const T* ret = boost::any_cast<T>(&(m_data->m_any));
	return *ret;

}

template <typename T>
void VeritasEngine::ResourceHandle::SetData(T&& data)
{
	m_data->m_any = std::move(data);
}
