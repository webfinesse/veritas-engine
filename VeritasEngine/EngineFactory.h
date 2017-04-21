#pragma once

#include <memory>

#include "DynamicLibraryHelper.h"

namespace VeritasEngine
{
	class Engine;

	std::unique_ptr<Engine> EXPORT CreateEngine();
}