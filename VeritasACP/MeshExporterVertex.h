#pragma once
#include "../VeritasEngineBase/MathTypes.h"

namespace VeritasACP
{
	struct MeshExporterVertex
	{
		VeritasEngine::Float3 Position;
		VeritasEngine::Float3 Normal;
		VeritasEngine::Float3 Tangent;
		VeritasEngine::Float3 Binormal;
		VeritasEngine::Float2 TextureCoordinates;
		std::vector<std::byte> JointIndicies;
		std::vector<float> JointWeights;
	};
}
