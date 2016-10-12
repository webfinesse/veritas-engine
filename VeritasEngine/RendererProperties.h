#ifndef H_RENDERER_PROPERTIES
#define H_RENDERER_PROPERTIES

#include "GameObjectProperty.h"
#include "MeshInstance.h"

namespace VeritasEngine
{
	class ResourceHandle;

	namespace RendererProperties
	{
		extern GameObjectProperty<MeshInstance> ObjectMesh;
		extern GameObjectProperty<ResourceHandle*> ResourcedMesh;
	};
}

#endif