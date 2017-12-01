#pragma once

#include <vector>

#include "../VeritasEngine/StringHash.h"
#include "MathTypes.h"



namespace VeritasEngine
{
	template <typename T>
	struct Channel
	{
		T Data;
		float Time;

		template <class Archive>
		void serialize(Archive& archive)
		{
			archive(Data, Time);
		}
	};

	struct BoneInformation
	{
		int BoneIndex;
		std::vector<Channel<VeritasEngine::Float3>> ScaleChannel;
		std::vector<Channel<VeritasEngine::Float3>> TranslationChannel;
		std::vector<Channel<VeritasEngine::Quaternion>> RotationChannel;

		template <class Archive>
		void serialize(Archive& archive)
		{
			archive(BoneIndex, ScaleChannel, TranslationChannel, RotationChannel);
		}
	};

	struct Animation
	{
		StringHash HashedName;
		float Duration;
		std::vector<BoneInformation> BoneInfo;
		std::string Name;

		template <class Archive>
		void serialize(Archive& archive)
		{
			archive(HashedName, Duration, BoneInfo, Name);
		}
	};
}
