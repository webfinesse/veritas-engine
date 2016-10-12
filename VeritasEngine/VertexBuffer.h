#ifndef H_VERTEXBUFFER
#define H_VERTEXBUFFER

#include <memory>

#include "../Includes/gsl/include/gsl.h"
#include "../VeritasEngineBase/SmallObject.h"

namespace VeritasEngine
{
	class BufferIndicies;

	class VertexBuffer : SmallObject<>
	{
	public:
		VertexBuffer();
		VertexBuffer(size_t sizeOfVertex);
		~VertexBuffer();

		BufferIndicies AddVerticies(gsl::byte* verticies, size_t numVerticies);
		size_t GetVertexSize() const;

		void* GetNativeBuffer();
	private:
		struct Impl;
		std::unique_ptr<Impl> m_impl;
	};
}

#endif