#ifndef H_SKINNEDVERTEX
#define H_SKINNEDVERTEX

#include <vector>

#include "../VeritasEngineBase/MathTypes.h"

constexpr unsigned int MAX_JOINT_WEIGHTS{ 4 };

namespace VeritasEngine
{
	struct SkinnedVertex
	{
		static const unsigned int Type{ 0x4f34e823 };
		Float3 Position; // 0 - 12
		Float3 Normal; // 12 - 24
		Float3 Tangent; // 24 - 36
		Float3 Binormal; // 36 - 48
		Float2 TextureCoordinates; // 48 - 56
		int JointIndicies[MAX_JOINT_WEIGHTS]; // 56 - 72
		Float3 JointWeights; // 72 - 84
		//std::vector<unsigned char> JointIndicies;
		//std::vector<float> JointWeights;

		template <class Archive>
		void serialize(Archive& archive)
		{
			archive(Position, Normal, Tangent, Binormal, TextureCoordinates, JointIndicies, JointWeights);
		}
	};
}

#endif