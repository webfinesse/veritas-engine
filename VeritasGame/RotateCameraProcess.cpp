#include "RotateCameraProcess.h"

#include "../VeritasEngine/GameObjectProperty.h"
#include "../VeritasEngineBase/MathTypes.h"
#include "../VeritasEngine/GamePropertyManager.h"
#include "../VeritasEngine/GameObjectPropertyKeys.h"

using namespace std;

RotateCameraProcess::RotateCameraProcess(VeritasEngine::GamePropertyManager& gamePropertyManager, float cameraDistance, std::chrono::seconds interval)
	: m_cameraDistance{ cameraDistance }, m_clock { interval, 0s, 1.0f, false }
{
	m_cameraDistanceProperty = gamePropertyManager.GetProperty<VeritasEngine::Float3>(VeritasEngine::GameObjectPropertyKeys::CameraPosition);
}


void RotateCameraProcess::OnInitialized()
{
	Process::OnInitialized();
}

void RotateCameraProcess::OnUpdate(VeritasEngine::TimeDuration delta)
{
	const auto cameraPosition = m_cameraDistanceProperty->GetProperty(1);

	const auto percentComplete = m_clock.GetPercentComplete();

	const float nextRadians = glm::three_over_two_pi<float>() + (glm::two_pi<float>() * percentComplete);
	
	cameraPosition->x = m_cameraDistance * cos(nextRadians);
	cameraPosition->z = m_cameraDistance * sin(nextRadians);
	
	m_clock.Update(delta);
}