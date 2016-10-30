#include "SceneGraphProperties.h"
#include "StringHash.h"

const VeritasEngine::StringHash sceneNodeTypeHash = VESTRINGHASH("SceneNodeType");
const VeritasEngine::StringHash worldPositionHash = VESTRINGHASH("WorldPosition");
const VeritasEngine::StringHash cameraTargetHash = VESTRINGHASH("CameraTarget");
const VeritasEngine::StringHash cameraPositionHash = VESTRINGHASH("CameraPosition");
const VeritasEngine::StringHash lightHash = VESTRINGHASH("Light");

VeritasEngine::GameObjectProperty<VeritasEngine::SceneNodeType> VeritasEngine::SceneGraphProperties::Type("Scene Node Type", sceneNodeTypeHash);
VeritasEngine::GameObjectProperty<VeritasEngine::Matrix4x4> VeritasEngine::SceneGraphProperties::WorldPosition("World Position", worldPositionHash);

VeritasEngine::GameObjectProperty<VeritasEngine::Float3> VeritasEngine::SceneGraphProperties::CameraTarget("Camera Target", cameraTargetHash);
VeritasEngine::GameObjectProperty<VeritasEngine::Float3> VeritasEngine::SceneGraphProperties::CameraPosition("Camera Position", cameraPositionHash);

VeritasEngine::GameObjectProperty<VeritasEngine::Light> VeritasEngine::SceneGraphProperties::Light("Light", lightHash);