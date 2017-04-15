#ifndef H_SKELETON
#define H_SKELETON

#include <vector>

#include "cereal/types/vector.hpp"

#include "SkeletonJoint.h"

namespace VeritasEngine
{
	struct Skeleton
	{
		std::vector<SkeletonJoint> Joints;

		template <class Archive>
		void serialize(Archive& archive)
		{
			archive(Joints);
		}
	};
}

#endif