#pragma once

namespace VeritasEngine
{
	class VertexBufferManager;
	class IndexBufferManager;
	class Scene;
	class Renderer;

	class IRenderingServices
	{
	public:
		virtual ~IRenderingServices() = default;
		virtual VertexBufferManager& GetVertexBufferManager() const = 0;
		virtual IndexBufferManager& GetIndexBufferManager() const = 0;
		virtual Scene& GetScene() const = 0;
		virtual Renderer& GetRenderer() const = 0;
	};
}