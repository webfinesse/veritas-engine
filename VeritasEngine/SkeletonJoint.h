#ifndef H_SKELETONJOINT
#define H_SKELETONJOINT

#include <string>

#include "../VeritasEngineBase/MathTypes.h"

#include "../Includes/cereal-1.1.2/include/cereal/types/string.hpp"

namespace VeritasEngine
{
	struct SkeletonJoint
	{
		std::string Name;
		int ParentIndex;
		Matrix4x4 InverseBindPose;

		template <class Archive>
		void serialize(Archive& archive)
		{
			archive(Name, ParentIndex, InverseBindPose);
		}
	};
}

#endif