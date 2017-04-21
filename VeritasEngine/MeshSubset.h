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

		void SetVertices(VertexBuffer* vertexBuffer, unsigned char* verticies, unsigned int numOfVertices);
		std::size_t GetVertexSize() const;
		unsigned int GetVertexCount() const;
		VertexBuffer& GetVertexBuffer() const;
		unsigned int GetVertexBufferBaseIndex() const;


		void SetIndicies(IndexBuffer* indexBuffer, unsigned int* indicies, unsigned int numOfIndicies);
		unsigned int IndexCount() const;
		IndexBuffer& GetIndexBuffer() const;
		unsigned int GetIndexOffset() const;

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