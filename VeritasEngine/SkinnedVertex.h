#pragma once

#include "../VeritasEngineBase/MathTypes.h"
#include "SmallObject.h"

namespace VeritasEngine
{
	struct SkinnedVertex final : SmallPODObject<>
	{
		static const unsigned int Type{ 0x4f34e823 };
		Float3 Position; // 0 -96
		Float3 Normal; // 96 - 192
		Float2 TextureCoordinates; //192 - 256
		unsigned char JointIndicies[4]; // 256 - 288
		Float3 JointWeights; // 288 - 384

		template <class Archive>
		void serialize(Archive& archive)
		{
			archive(Position, Normal, TextureCoordinates);
		}
	};
}
