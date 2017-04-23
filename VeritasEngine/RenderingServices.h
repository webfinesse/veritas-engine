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
		RenderingServices(std::shared_ptr<IScene> scene);
		~RenderingServices() override;

		RenderingServices(RenderingServices&& other) noexcept;
		RenderingServices& operator=(RenderingServices&& other) noexcept;

		VertexBufferManager& GetVertexBufferManager() const override;
		IndexBuffer& GetIndexBuffer() const override;
		IScene& GetScene() const override;
		Renderer& GetRenderer() const override;
	private:
		struct Impl;
		std::unique_ptr<Impl> m_impl;
	};
}

#endif

