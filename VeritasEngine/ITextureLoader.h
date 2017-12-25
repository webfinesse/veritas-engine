#pragma once

#include <iosfwd>

namespace VeritasEngine
{
	class ResourceData;

	class ITextureLoader
	{
	public:
		virtual ~ITextureLoader() noexcept = default;

		virtual void Load(std::istream& data, ResourceData& handle) const = 0;
	};
}