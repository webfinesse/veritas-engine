#pragma once

#include <memory>
#include <string>

#include "DynamicLibraryHelper.h"
#include "../VeritasEngineBase/ResourceId.h"
#include "SmallObject.h"

#pragma warning(push)
#pragma warning(disable: 4251)

namespace VeritasEngine
{
	class ResourceHandle;

	class EXPORT ResourceManager : public SmallObject<>
	{
	public:
		ResourceManager();
		~ResourceManager();

		void Init(const std::string& path);

		ResourceHandle* GetResource(const ResourceId& resourcePath);

	private:
		struct Impl;
		std::unique_ptr<Impl> m_impl;
	};
}

#pragma warning(pop)
