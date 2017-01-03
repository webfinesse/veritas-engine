#pragma once
#include "../VeritasGame/RotateObjectProcess.h"
#include <string>
#include "../VeritasEngineBase/MathTypes.h"

namespace VeritasEngine
{
	struct SkeletonJoint final : SmallPODObject<>
	{
		std::string Name;
		int ParentIndex;
		Matrix4x4 InverseBindPose;
	};
}
