#include "EngineFactory.h"

#include "Engine.h"
#include "EngineDepenencyInjector.h"
#include "../VeritasEngine_Windows/WIndowsDependencyInjector.h"

std::unique_ptr<VeritasEngine::Engine> VeritasEngine::CreateEngine()
{
	return di::make_injector(CreateEngineDependencyInjector(), CreateWindowsWindowsDependencyInjector()).create<unique_ptr<Engine>>();
}
