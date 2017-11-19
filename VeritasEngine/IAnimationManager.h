#ifndef H_IANIMATIONMANAGER
#define H_IANIMATIONMANAGER

#include "../VeritasEngineBase/GameObjectHandle.h"
#include "../VeritasEngineBase/MathTypes.h"
#include "StringHash.h"
#include "ClockUnits.h"
#include "DynamicLibraryHelper.h"

namespace VeritasEngine
{
	
	class EXPORT IAnimationManager
	{
	public:
		virtual ~IAnimationManager() = default;

		virtual void AddAnimaton(GameObjectHandle handle, StringHash animationName, bool isLooped, float timeScale = 1) = 0;
		virtual void CalculateSkinningPalettes(TimeDuration update) = 0;
		virtual const Matrix4x4* GetSkinningPalette(GameObjectHandle handle) = 0;
	};
}

#endif