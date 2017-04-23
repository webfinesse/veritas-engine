#pragma once

#include <istream>
#include <memory>

#include "../VeritasEngine/ITextureLoader.h"

namespace VeritasEngine 
{
	class ResourceHandle;
	class DirectXState;

	class TextureResourceLoaderImpl: public ITextureLoader
	{
	public:
		TextureResourceLoaderImpl(std::shared_ptr<DirectXState> dxState);
		~TextureResourceLoaderImpl() override = default;

		void Load(std::istream& data, ResourceHandle& handle) const override;

	private:
		std::shared_ptr<DirectXState> m_dxState;
	};
}
