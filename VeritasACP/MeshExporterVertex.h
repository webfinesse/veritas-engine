#pragma once
#include "../VeritasEngineBase/MathTypes.h"

//constexpr unsigned int MAX_JOINT_WEIGHTS{ 4 };

namespace VeritasACP
{
	struct MeshExporterVertex
	{
		VeritasEngine::Float3 Position;
		VeritasEngine::Float3 Normal;
		VeritasEngine::Float3 Tangent;
		VeritasEngine::Float3 Binormal;
		VeritasEngine::Float2 TextureCoordinates;
		std::vector<unsigned char> JointIndicies;
		std::vector<float> JointWeights;
	};
}
