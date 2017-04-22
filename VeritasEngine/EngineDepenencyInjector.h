#pragma once

#include <boost/di.hpp>

#include "ProcessManager.h"

#include "DeserializeMapping.h"

#include "RenderingServices.h"

#include "ResourceManager.h"
#include "MeshResourceLoader.h"
#include "AnimatedMeshResourceLoader.h"
#include "MaterialResourceLoader.h"
#include "TextureResourceLoader.h"
#include "SkeletonResourceLoader.h"

#include "WorldSetup.h"
#include "GameClock.h"

namespace di = boost::di;

inline auto CreateEngineDependencyInjector()
{
	return di::make_injector(
		di::bind<VeritasEngine::IGameClock>().to<VeritasEngine::GameClock>(),
		di::bind<VeritasEngine::IProcessManager>().to<VeritasEngine::ProcessManager>(),
		di::bind<VeritasEngine::IDeserializeMapping>().to<VeritasEngine::DeserializeMapping>(),
		di::bind<VeritasEngine::IWorldSetup>().to<VeritasEngine::WorldSetup>(),
		di::bind<VeritasEngine::IRenderingServices>().to<VeritasEngine::RenderingServices>(),
		di::bind<VeritasEngine::IResourceLoader*[]>().to<VeritasEngine::MeshResourceLoader, VeritasEngine::AnimatedMeshResourceLoader, VeritasEngine::MaterialResourceLoader, VeritasEngine::TextureResourceLoader, VeritasEngine::SkeletonResourceLoader>(),
		di::bind<VeritasEngine::IResourceManager>().to<VeritasEngine::ResourceManager>()
	);
}