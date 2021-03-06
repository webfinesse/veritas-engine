#ifndef H_IMESHSHADER
#define H_IMESHSHADER

namespace VeritasEngine
{
	struct PassBuffer;
	struct PerObjectBufferDescription;
	class IResourceManager;

	class IMeshShader
	{
	public:
		virtual void Activate() = 0;
		virtual void Deactivate() = 0;
		virtual void Init(std::shared_ptr<IResourceManager> resourceManager) = 0;

		virtual void SetPassParameters(PassBuffer& passBuffer) = 0;
		virtual void SetPerObjectBuffer(const PerObjectBufferDescription& buffer) = 0;

		virtual ~IMeshShader() noexcept = default;
	};
}

#endif
