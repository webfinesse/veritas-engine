#ifndef H_MESHSUBSET
#define H_MESHSUBSET

#include <memory>
#include "SmallObject.h"

#include "VertexTypeHandle.h"

namespace VeritasEngine
{
	class VertexBuffer;
	class IndexBuffer;
	class ResourceHandle;

	class MeshSubset: public SmallObject<>
	{
	public:
		MeshSubset();
		~MeshSubset();

		void SetVertices(VertexBuffer* vertexBuffer, unsigned char* verticies, size_t numOfVertices);
		size_t GetVertexSize() const;
		size_t GetVertexCount() const;
		VertexBuffer& GetVertexBuffer() const;
		size_t GetVertexBufferBaseIndex() const;


		void SetIndicies(IndexBuffer* indexBuffer, unsigned int* indicies, size_t numOfIndicies);
		size_t IndexCount() const;
		IndexBuffer& GetIndexBuffer() const;
		size_t GetIndexOffset() const;

		void SetMaterial(const ResourceHandle* const material);

		const ResourceHandle* const GetMaterial() const;

		MeshSubset(const MeshSubset& rhs);
		MeshSubset& operator=(MeshSubset& rhs);

	private:
		struct Impl;
		std::unique_ptr<Impl> m_impl;
	};
}

#endif