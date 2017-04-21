#pragma once

#include "DynamicLibraryHelper.h"

namespace VeritasEngine
{
	class Engine;

	class EXPORT IWorldSetup
	{
	public:
		virtual ~IWorldSetup() = default;
		virtual void Init(Engine& engine) = 0;
		virtual void LoadFile(const char* filename) = 0;
	};
}