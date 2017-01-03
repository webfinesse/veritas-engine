#pragma once
#include "../VeritasEngine/SmallObject.h"
#include <vector>
#include "SkeletonJoint.h"

namespace VeritasEngine
{
	struct Skeleton final : SmallPODObject<>
	{
		vector<SkeletonJoint> Joints;
	};
}
