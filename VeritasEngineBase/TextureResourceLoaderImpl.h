#pragma once

#include <istream>

namespace VeritasEngine {
	class ResourceHandle;

	class TextureResourceLoaderImpl
	{
	public:
		TextureResourceLoaderImpl() = default;
		~TextureResourceLoaderImpl() = default;

		void Load(std::istream& data, ResourceHandle& handle) const;
	};
}
