#pragma once
#include <variant>

#include "Skeleton.h"
#include "MeshInstance.h"
#include "../VeritasEngine_Windows/DirectXTextureData.h"
#include "../VeritasEngineBase/MaterialInstance.h"

namespace VeritasEngine
{
	class ResourceData
	{
	public:
		ResourceData() noexcept = default;
		ResourceData(const ResourceData& rhs) = default;

		ResourceData(ResourceData&& rhs) noexcept
			: m_data{ std::move(rhs.m_data) }
		{
		}

		~ResourceData() noexcept = default;

		template <typename T>
		const T& GetData() const
		{
			return std::get<T>(m_data);
		}
		
		template <typename T>
		void SetData(T&& data)
		{
			m_data = std::move(data);
		}
		
		bool IsFullLoaded() const
		{
			return m_isFullyLoaded;
		}

		ResourceData& operator=(const ResourceData& rhs)
		{
			if (this != &rhs)
			{
				m_data = rhs.m_data;
			}

			return *this;
		}

		ResourceData& operator=(ResourceData&& rhs) noexcept
		{
			if (this != &rhs)
			{
				m_data = std::move(rhs.m_data);
			}

			return *this;
		}

		friend class ResourceManager;

	private:
		std::variant<Skeleton, MeshInstance, DirectXTextureData, MaterialInstance> m_data{};
		bool m_isFullyLoaded{ false };
	};
}
