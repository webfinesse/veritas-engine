#ifndef H_MESHSUBSET
#define H_MESHSUBSET

#include <memory>
#include "SmallObject.h"

namespace VeritasEngine
{
	struct SerializedMeshSubset;
	class ResourceHandle;
	class BufferIndicies;

	class MeshSubset: public SmallObject<>
	{
	public:
		MeshSubset(const SerializedMeshSubset& subset);
		~MeshSubset();

		const BufferIndicies& GetVertexBufferIndicies() const;
		const BufferIndicies& GetIndexBufferIndicies() const;

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