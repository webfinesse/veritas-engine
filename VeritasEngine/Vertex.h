#ifndef H_VERTEX
#define H_VERTEX

#include "../VeritasEngineBase/MathTypes.h"
#include "SmallObject.h"

namespace VeritasEngine
{
	struct Vertex
	{
		static const unsigned int Type{ 0x4f34e757 };
		Float3 Position; // 0-96
		Float3 Normal; // 96 - 192
		Float3 Tangent; // 192 - 256
		Float3 Binormal; // 256 - 352
		Float2 TextureCoordinates; // 352-416

		template <class Archive>
		void serialize(Archive& archive)
		{
			archive(Position, Normal, Tangent, Binormal, TextureCoordinates);
		}
	};
}

#endif