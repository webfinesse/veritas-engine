#ifndef H_RENDERINGSERVICES
#define H_RENDERINGSERVICES

#include <memory>

#include "SmallObject.h"
#include "IRenderingServices.h"

namespace VeritasEngine
{
	class RenderingServices : public SmallObject<>, public IRenderingServices
	{
	public:
		RenderingServices();
		~RenderingServices() override;
		VertexBufferManager& GetVertexBufferManager() const override;
		IndexBufferManager& GetIndexBufferManager() const override;
		Scene& GetScene() const override;
		Renderer& GetRenderer() const override;
	private:
		struct Impl;
		std::unique_ptr<Impl> m_impl;
	};
}

#endif

