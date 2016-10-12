#ifndef H_ROTATE_OBJECT_PROCESS
#define H_ROTATE_OBJECT_PROCESS

#include "../VeritasEngine/Process.h"
#include "../VeritasEngineBase/GameObjectHandle.h"

using namespace VeritasEngine;

class RotateObjectProcess : public VeritasEngine::Process
{
public:
	RotateObjectProcess(GameObjectHandle handle);
	void OnInitialized() override;
	void OnUpdate(float delta) override;
private:
	float lastUpdate;
	GameObjectHandle m_handle;
};

#endif