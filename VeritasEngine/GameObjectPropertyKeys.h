#pragma once

#include "StringHash.h"

namespace VeritasEngine
{
	namespace GameObjectPropertyKeys
	{
		constexpr StringHash ObjectMesh{ VESTRINGHASH("ObjectMesh") };
		constexpr StringHash ResourcedMesh{ VESTRINGHASH("ResourcedMesh") };

		constexpr StringHash SceneNodeType{ VESTRINGHASH("SceneNodeType") };
		constexpr StringHash WorldPosition{ VESTRINGHASH("WorldPosition") };

		constexpr StringHash CameraTarget{ VESTRINGHASH("CameraTarget") };
		constexpr StringHash CameraPosition{ VESTRINGHASH("CameraPosition") };

		constexpr StringHash Light{ VESTRINGHASH("Light") };
	}
}
