#ifndef H_ROTATE_CAMERA_PROCESS
#define H_ROTATE_CAMERA_PROCESS

#include "../VeritasEngine/Process.h"

class RotateCameraProcess : public VeritasEngine::Process
{
public:
	RotateCameraProcess(float cameraDistance, chrono::milliseconds interval);

	void OnInitialized() override;
	void OnUpdate(VeritasEngine::TimeDuration delta) override;
private:
	float m_cameraDistance;
	VeritasEngine::TimeDuration m_interval;
	VeritasEngine::TimeDuration m_lastUpdate;
};

#endif