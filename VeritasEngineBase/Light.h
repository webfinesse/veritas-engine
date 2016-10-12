#ifndef H_DIRECTIONAL_LIGHT
#define H_DIRECTIONAL_LIGHT

#include "MathTypes.h"

namespace VeritasEngine
{
	enum class LightType: int
	{
		Directional = 1,
		Spotlight = 2,
		Point = 3,
	};

	struct Light
	{
		static const size_t MAX_LIGHTS = 8;

		Float4 Position; // 0 - 16 bytes
		Float4 Direction; // 16 - 32 bytes
		Float4 Color; // 32 - 48 bytes		
		float SpotAngle; // 48-52 bytes
		float ConstantAttenuation; // 52 - 56 byte
		float LinearAttenuation; // 56 - 60 bytes
		float QuadraticAttenuation; // 60 - 64 bytes
		LightType Type; // 64 - 68 bytes
		int Enabled; // 68 - 72 bytes
		char pad[8]; // 72 - 80 bytes
	};
}

#endif