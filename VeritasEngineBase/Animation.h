#pragma once

#include <vector>

#include "../VeritasEngine/StringHash.h"
#include "MathTypes.h"



namespace VeritasEngine
{
	struct Keyframe
	{
		Float3 Scale; // 0 - 96
		Quaternion Rotation; // 96 - 224
		Float3 Translation; // 224 - 320
		float TimeSample; // 320 - 352

		template <class Archive>
		void serialize(Archive& archive)
		{
			archive(Scale, Rotation, Translation, TimeSample);
		}
	};

	struct BoneInformation
	{
		size_t BoneIndex;
		std::vector<Keyframe> Keyframes;

		template <class Archive>
		void serialize(Archive& archive)
		{
			archive(BoneIndex, Keyframes);
		}
	};

	struct Animation
	{
		StringHash HashedName;
		float Duration;
		std::vector<BoneInformation> BoneInfo;

		template <class Archive>
		void serialize(Archive& archive)
		{
			archive(HashedName, Duration, BoneInfo);
		}
	};
}
