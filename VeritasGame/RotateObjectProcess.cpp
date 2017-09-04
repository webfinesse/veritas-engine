#include "RotateObjectProcess.h"

#include "../VeritasEngine/GameObjectProperty.h"
#include "../VeritasEngineBase/GameObjectHandle.h"
#include "../VeritasEngine/GameObjectPropertyKeys.h"

RotateObjectProcess::RotateObjectProcess(VeritasEngine::GamePropertyManager& gamePropertyManager, GameObjectHandle handle)
	: m_handle(handle)
{
	m_worldPositionProperty = gamePropertyManager.GetProperty<Matrix4x4>(GameObjectPropertyKeys::WorldPosition);
}

void RotateObjectProcess::OnInitialized()
{
	Process::OnInitialized();
}

void RotateObjectProcess::OnUpdate(TimeDuration delta)
{
	auto worldMatrix = m_worldPositionProperty->GetProperty(m_handle);

	auto count = std::chrono::duration_cast<std::chrono::seconds>(delta).count();
	auto rotationMatrix = VeritasEngine::MathHelpers::CreateRotationRollPitchYawMatrix(static_cast<float>(0.5 * count), 0.0, 0.0);

	auto result = *worldMatrix * rotationMatrix;
	
	*worldMatrix = result;
}