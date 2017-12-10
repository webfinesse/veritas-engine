#pragma once
#include <any>

namespace VeritasEngine
{
	class ResourceHandle
	{
	public:
		ResourceHandle()
		{
		}

		ResourceHandle(const ResourceHandle& rhs)
			: m_any{ rhs.m_any }
		{
		}

		ResourceHandle( ResourceHandle&& rhs) noexcept
			: m_any{ std::move(rhs.m_any) }
		{
		}

		~ResourceHandle()
		{
		}

		template <typename T>
		const T& GetData() const
		{
			const T* ret = std::any_cast<T>(&m_any);
			return *ret;
		}
		
		template <typename T>
		void SetData(T&& data)
		{
			m_any = std::move(data);
		}		

		ResourceHandle& operator=(const ResourceHandle& rhs)
		{
			if (this != &rhs)
			{
				m_any = rhs.m_any;
			}

			return *this;
		}

		ResourceHandle& operator=(ResourceHandle&& rhs) noexcept
		{
			if (this != &rhs)
			{
				m_any = std::move(rhs.m_any);
			}

			return *this;
		}

	private:
		std::any m_any{};
	};
}