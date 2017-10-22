#pragma once

#include "StringHash.h"

namespace VeritasEngine
{
	namespace GameObjectPropertyKeys
	{
		constexpr auto ObjectMesh{ VESTRINGHASH("ObjectMesh") };
		constexpr auto ResourcedMesh{ VESTRINGHASH("ResourcedMesh") };

		constexpr auto SceneNodeType{ VESTRINGHASH("SceneNodeType") };
		constexpr auto WorldPosition{ VESTRINGHASH("WorldPosition") };

		constexpr auto CameraTarget{ VESTRINGHASH("CameraTarget") };
		constexpr auto CameraPosition{ VESTRINGHASH("CameraPosition") };

		constexpr auto Light{ VESTRINGHASH("Light") };
	}
}
