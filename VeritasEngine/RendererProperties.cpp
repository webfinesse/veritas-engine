#include "RendererProperties.h"

const VeritasEngine::StringHash objectMeshHash = VESTRINGHASH("ObjectMesh");
const VeritasEngine::StringHash resourcedMeshHash = VESTRINGHASH("ResourcedMesh");

VeritasEngine::GameObjectProperty<VeritasEngine::MeshInstance> VeritasEngine::RendererProperties::ObjectMesh("Object Mesh", objectMeshHash);
VeritasEngine::GameObjectProperty<VeritasEngine::ResourceHandle*> VeritasEngine::RendererProperties::ResourcedMesh("Resourced Mesh", resourcedMeshHash);