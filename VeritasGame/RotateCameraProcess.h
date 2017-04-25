#ifndef H_ROTATE_CAMERA_PROCESS
#define H_ROTATE_CAMERA_PROCESS

#include "../VeritasEngine/Process.h"
#include "../VeritasEngine/AnimationClock.h"
#include "../VeritasEngine/GameObjectProperty.h"

class RotateCameraProcess : public VeritasEngine::Process
{
public:
	RotateCameraProcess(VeritasEngine::GamePropertyManager& propertyManager, float cameraDistance, chrono::seconds duration);

	void OnInitialized() override;
	void OnUpdate(VeritasEngine::TimeDuration delta) override;
private:
	float m_cameraDistance;
	VeritasEngine::AnimationClock m_clock;
	VeritasEngine::GameObjectProperty<VeritasEngine::Float3>* m_cameraDistanceProperty;
};

#endif