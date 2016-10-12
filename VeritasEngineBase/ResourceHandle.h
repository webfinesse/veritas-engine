#pragma once

#include <memory>
#include "SmallObject.h"

namespace VeritasEngine
{
	class ResourceHandle : public SmallPODObject<>
	{
	public:
		ResourceHandle();

		~ResourceHandle();

		template <typename T>
		const T& GetData() const;
		
		template <typename T>
		void SetData(T&& data);

		ResourceHandle(const ResourceHandle& rhs);
		ResourceHandle& operator=(const ResourceHandle& handle);

	private:
		struct Impl;
		std::unique_ptr<Impl> m_data;
	};
}

#include "ResourceHandle_details.h"