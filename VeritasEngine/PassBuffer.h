#ifndef H_CAMERABUFFER
#define H_CAMERABUFFER

#include "../VeritasEngineBase/MathTypes.h"
#include "../VeritasEngineBase/Light.h"

namespace VeritasEngine
{
	struct PassBuffer
	{
		Matrix4x4 ViewMatrix; // 0 - 64
		Matrix4x4 ProjectionMatrix; // 64 - 128
		Float3 CameraPosition; // 128 - 140
		Light Lights[Light::MAX_LIGHTS]; // 8 * 80 = 640, 140 - 780
		float pad; // 780-784 
	};
}

#endif
