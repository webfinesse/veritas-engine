#ifndef H_MESHINSTANCE
#define H_MESHINSTANCE

#include <memory>
#include <vector>
#include "../VeritasEngineBase/MathTypes.h"

#include "SmallObject.h"
#include "../VeritasEngineBase/ResourceId.h"

namespace VeritasEngine
{
	class VertexBuffer;
	class MeshSubset;
	class MeshNode;
	struct Skeleton;
	struct Animation;
	class IVertexBuffer;
	class IIndexBuffer;
	struct SerializedMeshSubset;

	class MeshInstance: public SmallObject<> {
	public:
		MeshInstance();
		~MeshInstance();

		MeshNode& GetRootNode() const;

		void SetVertices(IVertexBuffer* vertexBuffer, unsigned char* verticies, size_t numOfVertices);
		size_t GetVertexSize() const;
		size_t GetVertexBufferStartIndex() const;
		IVertexBuffer& GetVertexBuffer() const;

		void SetIndicies(IIndexBuffer* indexBuffer, unsigned int* indicies, size_t numOfIndicies);
		size_t GetIndexBufferStartIndex() const;
		IIndexBuffer& GetIndexBuffer() const;

		MeshSubset& CreateSubset(const SerializedMeshSubset& subset);
		MeshSubset& GetSubset(unsigned int index) const;
		size_t GetSubsetCount() const;

		void SetSkeleton(const ResourceHandle skeleton);
		ResourceHandle GetSkeleton() const;

		void SetAnimations(const std::vector<Animation>& animation);
		const std::vector<Animation>& GetAnimations() const;

		void SetGlobalInverseTransform(const Matrix4x4& transform);
		const Matrix4x4& GetGlobalInverseTransform() const;

		MeshInstance(const MeshInstance& rhs);
		MeshInstance& operator=(const MeshInstance& rhs);

	private:
		struct Impl;
		std::unique_ptr<Impl> m_impl;
	};
}

#endif