#ifndef H_ROTATE_OBJECT_PROCESS
#define H_ROTATE_OBJECT_PROCESS

#include "../VeritasEngine/Process.h"
#include "../VeritasEngineBase/GameObjectHandle.h"
#include "../VeritasEngine/GameObjectProperty.h"
#include "../VeritasEngine/GamePropertyManager.h"

using namespace VeritasEngine;

class RotateObjectProcess : public VeritasEngine::Process
{
public:
	RotateObjectProcess(VeritasEngine::GamePropertyManager& gamePropertyManager, GameObjectHandle handle);
	void OnInitialized() override;
	void OnUpdate(TimeDuration delta) override;
private:
	GameObjectHandle m_handle;
	GameObjectProperty<Matrix4x4>* m_worldPositionProperty;
};

#endif