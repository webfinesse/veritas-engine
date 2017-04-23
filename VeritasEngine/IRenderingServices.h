#pragma once

namespace VeritasEngine
{
	class VertexBufferManager;
	class IndexBufferManager;
	class IScene;
	class Renderer;

	class IRenderingServices
	{
	public:
		virtual ~IRenderingServices() noexcept = default;
		virtual VertexBufferManager& GetVertexBufferManager() const = 0;
		virtual IndexBufferManager& GetIndexBufferManager() const = 0;
		virtual IScene& GetScene() const = 0;
		virtual Renderer& GetRenderer() const = 0;
	};
}