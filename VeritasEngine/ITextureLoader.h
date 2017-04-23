#pragma once

#include <iosfwd>

namespace VeritasEngine
{
	class ResourceHandle;

	class ITextureLoader
	{
	public:
		virtual ~ITextureLoader() noexcept = default;

		virtual void Load(std::istream& data, ResourceHandle& handle) const = 0;
	};
}