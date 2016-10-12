#ifndef H_ROTATE_CAMERA_PROCESS
#define H_ROTATE_CAMERA_PROCESS

#include "../VeritasEngine/Process.h"

class RotateCameraProcess : public VeritasEngine::Process
{
public:
	void OnInitialized() override;
	void OnUpdate(float delta) override;
private:
	float lastUpdate;
};

#endif