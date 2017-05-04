#ifndef H_VERTEX
#define H_VERTEX

#include "../VeritasEngineBase/MathTypes.h"
#include "SmallObject.h"

namespace VeritasEngine
{
	struct Vertex
	{
		static const unsigned int Type{ 0x4f34e757 };
		Float3 Position;
		Float3 Normal;
		Float3 Tangent;
		Float3 Binormal;
		Float2 TextureCoordinates;
		char pad[8];

		template <class Archive>
		void serialize(Archive& archive)
		{
			archive(Position, Normal, Tangent, Binormal, TextureCoordinates);
		}
	};
}

#endif