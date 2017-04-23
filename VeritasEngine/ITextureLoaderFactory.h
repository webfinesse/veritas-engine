#pragma once

#include <memory>

namespace VeritasEngine
{
	class ITextureLoader;

	class ITextureLoaderFactory
	{
	public:
		virtual ~ITextureLoaderFactory() noexcept = default;

		virtual std::shared_ptr<ITextureLoader> Create() = 0;
	};
}