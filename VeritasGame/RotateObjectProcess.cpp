#include "RotateObjectProcess.h"

#include "../VeritasEngine/GameObjectProperty.h"
#include "../VeritasEngineBase/GameObjectHandle.h"

#include "../VeritasEngine/SceneGraphProperties.h"

RotateObjectProcess::RotateObjectProcess(GameObjectHandle handle)
	: m_handle(handle)
{

}

void RotateObjectProcess::OnInitialized()
{
	this->lastUpdate = 0;
	Process::OnInitialized();
}

void RotateObjectProcess::OnUpdate(float delta)
{
	auto worldMatrix = VeritasEngine::SceneGraphProperties::WorldPosition.GetProperty(m_handle);
	
	auto rotationMatrix = VeritasEngine::MathHelpers::CreateRotationRollPitchYawMatrix(0.5 * delta, 0.0, 0.0);

	auto result = *worldMatrix * rotationMatrix;
	
	*worldMatrix = result;
	
	this->lastUpdate += delta;
}