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
	Process::OnInitialized();
}

void RotateObjectProcess::OnUpdate(TimeDuration delta)
{
	auto worldMatrix = VeritasEngine::SceneGraphProperties::WorldPosition.GetProperty(m_handle);

	auto count = chrono::duration_cast<chrono::seconds>(delta).count();
	auto rotationMatrix = VeritasEngine::MathHelpers::CreateRotationRollPitchYawMatrix(0.5 * count, 0.0, 0.0);

	auto result = *worldMatrix * rotationMatrix;
	
	*worldMatrix = result;
}