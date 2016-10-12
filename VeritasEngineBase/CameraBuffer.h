#ifndef H_CAMERABUFFER
#define H_CAMERABUFFER

#include "MathTypes.h"

namespace VeritasEngine
{
	struct CameraBuffer
	{
		Matrix4x4 m_viewMatrix;
		Matrix4x4 m_projectionMatrix;
		Float3 m_cameraPosition;
		float pad;
	};
}

#endif
