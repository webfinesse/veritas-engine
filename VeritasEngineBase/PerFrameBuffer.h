#ifndef H_PERFRAMEBUFFER
#define H_PERFRAMEBUFFER

#include "Light.h"

namespace VeritasEngine
{
	struct PerFrameBuffer
	{
		VeritasEngine::Light lights[Light::MAX_LIGHTS];
	};
}

#endif
