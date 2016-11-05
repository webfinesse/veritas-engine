#include "RotateCameraProcess.h"

#include "../VeritasEngine/GameObjectProperty.h"

#include "../VeritasEngine/SceneGraphProperties.h"
#include "../VeritasEngineBase/MathTypes.h"

RotateCameraProcess::RotateCameraProcess(float cameraDistance, chrono::seconds interval)
	: m_cameraDistance{ cameraDistance }, m_clock { interval, 0s, 1.0f, false }
{
	
}


void RotateCameraProcess::OnInitialized()
{
	Process::OnInitialized();
}

void RotateCameraProcess::OnUpdate(VeritasEngine::TimeDuration delta)
{
	auto cameraPosition = VeritasEngine::SceneGraphProperties::CameraPosition.GetProperty(1);

	auto percentComplete = m_clock.GetPercentComplete();

	float nextRadians = glm::three_over_two_pi<float>() + (glm::two_pi<float>() * percentComplete);
	
	cameraPosition->x = m_cameraDistance * cos(nextRadians);
	cameraPosition->z = m_cameraDistance * sin(nextRadians);
	
	m_clock.Update(delta);
}