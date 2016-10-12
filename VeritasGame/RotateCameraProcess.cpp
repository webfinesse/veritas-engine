#include "RotateCameraProcess.h"

#include "../VeritasEngine/GameObjectProperty.h"
#include "../VeritasEngineBase/GameObjectHandle.h"

#include "../VeritasEngine/SceneGraphProperties.h"
#include "../VeritasEngineBase/MathTypes.h"

void RotateCameraProcess::OnInitialized()
{
	this->lastUpdate = 0;
	Process::OnInitialized();
}

void RotateCameraProcess::OnUpdate(float delta)
{
	auto cameraPosition = VeritasEngine::SceneGraphProperties::CameraPosition.GetProperty(1);
	float currentRadians = asin(cameraPosition->z /150.0f);
	float currentDegrees = VeritasEngine::MathHelpers::ToDegrees(this->lastUpdate);

	float nextRadians = VeritasEngine::MathHelpers::ToRadians(currentDegrees);
	
	cameraPosition->x = 150.0f * cos(nextRadians);
	cameraPosition->z = 150.0f * sin(nextRadians);
	
	this->lastUpdate += delta;
}