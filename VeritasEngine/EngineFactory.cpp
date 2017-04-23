#include "EngineFactory.h"

#include "Engine.h"
#include "EngineDepenencyInjector.h"
#include "../VeritasEngine_Windows/WindowsDependencyInjector.h"

std::unique_ptr<VeritasEngine::Engine> VeritasEngine::CreateEngine()
{
	return di::make_injector(
							 CreateWindowsWindowsDependencyInjector(), 
							 CreateEngineDependencyInjector()
							).create<unique_ptr<Engine>>();
}
