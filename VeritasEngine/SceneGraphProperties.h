#ifndef H_SCENEGRAPHPROPERTIES
#define H_SCENEGRAPHPROPERTIES

#include "GameObjectProperty.h"
#include "SceneNodeType.h"
#include "../VeritasEngineBase/Light.h"

#include "DynamicLibraryHelper.h"
#include "../VeritasEngineBase/MathTypes.h"

namespace VeritasEngine
{
	namespace SceneGraphProperties
	{
		extern GameObjectProperty<VeritasEngine::SceneNodeType> EXPORT Type;
		extern GameObjectProperty<VeritasEngine::Matrix4x4> EXPORT WorldPosition;

		extern GameObjectProperty<VeritasEngine::Float3> EXPORT CameraTarget;
		extern GameObjectProperty<VeritasEngine::Float3> EXPORT CameraPosition;

		extern GameObjectProperty<VeritasEngine::Light> EXPORT Light;
	}
}

#endif


