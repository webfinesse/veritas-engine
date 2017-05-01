#ifndef H_SKINNEDVERTEX
#define H_SKINNEDVERTEX

#include <vector>

#include "../VeritasEngineBase/MathTypes.h"

namespace VeritasEngine
{
	struct SkinnedVertex
	{
		static const unsigned int Type{ 0x4f34e823 };
		Float3 Position; // 0 -96
		Float3 Normal; // 96 - 192
		Float2 TextureCoordinates; //192 - 256
		//unsigned char JointIndicies[4]; // 256 - 288
		//Float3 JointWeights; // 288 - 384
		std::vector<unsigned char> JointIndicies;
		std::vector<float> JointWeights;

		template <class Archive>
		void serialize(Archive& archive)
		{
			archive(Position, Normal, TextureCoordinates, JointIndicies, JointWeights);
		}
	};
}

#endif