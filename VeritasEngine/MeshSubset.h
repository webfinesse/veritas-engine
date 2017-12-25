#ifndef H_MESHSUBSET
#define H_MESHSUBSET

#include <memory>
#include "SmallObject.h"
#include "../VeritasEngineBase/ResourceId.h"

namespace VeritasEngine
{
	struct SerializedMeshSubset;
	class BufferIndicies;

	class MeshSubset: public SmallObject<>
	{
	public:
		MeshSubset(const SerializedMeshSubset& subset);
		~MeshSubset();

		const BufferIndicies& GetVertexBufferIndicies() const;
		const BufferIndicies& GetIndexBufferIndicies() const;

		void SetMaterial(const ResourceHandle material);
		ResourceHandle GetMaterial() const;

		MeshSubset(const MeshSubset& rhs);
		MeshSubset& operator=(MeshSubset& rhs);

	private:
		struct Impl;
		std::unique_ptr<Impl> m_impl;
	};
}

#endif