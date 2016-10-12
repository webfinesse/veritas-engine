#include "SceneGraphProperties.h"

VeritasEngine::GameObjectProperty<VeritasEngine::SceneNodeType> VeritasEngine::SceneGraphProperties::Type((long)(0x2f548106), L"Scene Node Type", "SceneNodeType");
VeritasEngine::GameObjectProperty<VeritasEngine::Matrix4x4> VeritasEngine::SceneGraphProperties::WorldPosition((long)0x8e478f3d, L"World Position", "WorldPosition");

VeritasEngine::GameObjectProperty<VeritasEngine::Float3> VeritasEngine::SceneGraphProperties::CameraTarget((long)0x98fac11a, L"Camera Target", "CameraTarget");
VeritasEngine::GameObjectProperty<VeritasEngine::Float3> VeritasEngine::SceneGraphProperties::CameraPosition((long)0x98fac11b, L"Camera Position", "CameraPosition");

VeritasEngine::GameObjectProperty<VeritasEngine::Light> VeritasEngine::SceneGraphProperties::Light((long)0x98fac11c, L"Light", "Light");