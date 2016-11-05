#include "RotateCameraProcess.h"

#include "../VeritasEngine/GameObjectProperty.h"
#include "../VeritasEngineBase/GameObjectHandle.h"

#include "../VeritasEngine/SceneGraphProperties.h"
#include "../VeritasEngineBase/MathTypes.h"

RotateCameraProcess::RotateCameraProcess(float cameraDistance, chrono::milliseconds interval)
	: m_cameraDistance{ cameraDistance }, m_interval { interval }
{
	
}


void RotateCameraProcess::OnInitialized()
{
	this->m_lastUpdate = 0ms;
	Process::OnInitialized();
}

void RotateCameraProcess::OnUpdate(VeritasEngine::TimeDuration delta)
{
	auto cameraPosition = VeritasEngine::SceneGraphProperties::CameraPosition.GetProperty(1);

	auto percentComplete = fmod(this->m_lastUpdate.count(), this->m_interval.count()) / this->m_interval.count();

	float nextRadians = glm::two_pi<float>() * percentComplete;
	
	cameraPosition->x = m_cameraDistance * cos(nextRadians);
	cameraPosition->z = m_cameraDistance * sin(nextRadians);
	
	this->m_lastUpdate += delta;
}