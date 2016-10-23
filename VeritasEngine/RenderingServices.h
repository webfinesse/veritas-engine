#ifndef H_RENDERINGSERVICES
#define H_RENDERINGSERVICES

#include <memory>

#include "SmallObject.h"

namespace VeritasEngine
{
	class VertexBufferManager;
	class IndexBufferManager;
	class Scene;
	class Renderer;

	class RenderingServices : public SmallObject<>
	{
	public:
		RenderingServices();
		~RenderingServices();
		VertexBufferManager& GetVertexBufferManager() const;
		IndexBufferManager& GetIndexBufferManager() const;
		Scene& GetScene() const;
		Renderer& GetRenderer() const;
	private:
		struct Impl;
		std::unique_ptr<Impl> m_impl;
	};
}

#endif

