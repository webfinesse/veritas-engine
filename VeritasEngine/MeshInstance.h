#ifndef H_MESHINSTANCE
#define H_MESHINSTANCE

#include <memory>

#include "SmallObject.h"

namespace VeritasEngine
{
	class VertexBuffer;
	class MeshSubset;
	class MeshNode;
	struct Skeleton;

	class MeshInstance: public SmallObject<> {
	public:
		MeshInstance();
		~MeshInstance();

		MeshNode& GetRootNode() const;

		MeshSubset& CreateSubset();
		MeshSubset& GetSubset(unsigned int index) const;
		unsigned int GetSubsetCount() const;

		void SetSkeleton(const Skeleton& skeleton);
		const Skeleton& GetSkeleton() const;

		MeshInstance(const MeshInstance& rhs);
		MeshInstance& operator=(const MeshInstance& rhs);

	private:
		struct Impl;
		std::unique_ptr<Impl> m_impl;
	};
}

#endif