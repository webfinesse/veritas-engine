#ifndef H_MESHINSTANCE
#define H_MESHINSTANCE

#include <memory>
#include <vector>

#include "SmallObject.h"

namespace VeritasEngine
{
	class VertexBuffer;
	class MeshSubset;
	class MeshNode;
	struct Skeleton;
	struct Animation;
	class ResourceHandle;

	class MeshInstance: public SmallObject<> {
	public:
		MeshInstance();
		~MeshInstance();

		MeshNode& GetRootNode() const;

		MeshSubset& CreateSubset();
		MeshSubset& GetSubset(unsigned int index) const;
		size_t GetSubsetCount() const;

		void SetSkeleton(const ResourceHandle* skeleton);
		const ResourceHandle* GetSkeleton() const;

		void SetAnimations(const std::vector<Animation>& animation);
		const std::vector<Animation>& GetAnimations() const;

		MeshInstance(const MeshInstance& rhs);
		MeshInstance& operator=(const MeshInstance& rhs);

	private:
		struct Impl;
		std::unique_ptr<Impl> m_impl;
	};
}

#endif