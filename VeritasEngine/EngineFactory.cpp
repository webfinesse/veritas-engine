#include "EngineFactory.h"

#include "EngineDepenencyInjector.h"

std::unique_ptr<VeritasEngine::Engine> VeritasEngine::CreateEngine()
{
	return CreateEngineDependencyInjector().create<unique_ptr<Engine>>();
}
