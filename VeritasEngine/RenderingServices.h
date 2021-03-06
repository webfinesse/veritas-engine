#ifndef H_RENDERINGSERVICES
#define H_RENDERINGSERVICES

#include <memory>

#include "SmallObject.h"
#include "IRenderingServices.h"

namespace VeritasEngine
{
	class IScene;

	class RenderingServices : public SmallObject<>, public IRenderingServices
	{
	public:
		RenderingServices(std::shared_ptr<IScene> scene, std::shared_ptr<IIndexBuffer> indexBuffer, std::shared_ptr<IVertexBufferManager> vertexBufferManager, std::shared_ptr<IRenderer> renderer);
		~RenderingServices() override;

		RenderingServices(RenderingServices&& other) noexcept;
		RenderingServices& operator=(RenderingServices&& other) noexcept;

		IVertexBufferManager& GetVertexBufferManager() const override;
		IIndexBuffer& GetIndexBuffer() const override;
		IScene& GetScene() const override;
		IRenderer& GetRenderer() const override;
	private:
		struct Impl;
		std::unique_ptr<Impl> m_impl;
	};
}

#endif

