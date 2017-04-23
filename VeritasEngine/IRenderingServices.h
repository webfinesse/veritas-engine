#pragma once

namespace VeritasEngine
{
	class IVertexBufferManager;
	class IIndexBuffer;
	class IScene;
	class IRenderer;

	class IRenderingServices
	{
	public:
		virtual ~IRenderingServices() noexcept = default;
		virtual IVertexBufferManager& GetVertexBufferManager() const = 0;
		virtual IIndexBuffer& GetIndexBuffer() const = 0;
		virtual IScene& GetScene() const = 0;
		virtual IRenderer& GetRenderer() const = 0;
	};
}