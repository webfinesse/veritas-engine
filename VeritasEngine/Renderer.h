#ifndef H_RENDERER
#define H_RENDERER

#include <memory>

namespace VeritasEngine
{
	class MeshInstance;

	class Renderer
	{
	public:
		Renderer();
		~Renderer();

		void Init(void* osData, unsigned int bufferWidth, unsigned int bufferHeight);
		void Resize(unsigned int bufferWidth, unsigned int bufferHeight);
		void Clear();
		void RenderSubset(const MeshInstance& mesh, unsigned int subsetIndex) const;
		void Present();

		float GetAspectRatio() const;

	private:
		Renderer(Renderer& rhs) = delete;
		Renderer& operator=(Renderer& rhs) = delete;

		Renderer(Renderer&& rhs) = delete;
		Renderer& operator=(Renderer&& rhs) = delete;

		struct Impl;
		std::unique_ptr<Impl> m_impl;
	};
}

#endif