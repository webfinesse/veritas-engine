#ifndef H_IANIMATIONMANAGER
#define H_IANIMATIONMANAGER

#include "../VeritasEngineBase/GameObjectHandle.h"
#include "../VeritasEngineBase/MathTypes.h"
#include "StringHash.h"
#include "ClockUnits.h"
#include "DynamicLibraryHelper.h"

namespace VeritasEngine
{
	struct Job;
	class IResourceManager;

	class EXPORT IAnimationManager
	{
	public:
		virtual ~IAnimationManager() = default;

		virtual void Init(std::shared_ptr<IResourceManager> resourceManager) = 0;

		virtual void AddAnimaton(GameObjectHandle handle, StringHash animationName, bool isLooped, float timeScale = 1) = 0;
		virtual Job* CalculatePoses(TimeDuration update) = 0;
		virtual const Matrix4x4* GetGlobalPoses(GameObjectHandle handle) = 0;
	};
}

#endif