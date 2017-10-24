#pragma once

#include <../Includes/boost-di/di.hpp>

#include "ProcessManager.h"

#include "DeserializeMapping.h"

#include "Scene.h"
#include "RenderingServices.h"

#include "ResourceManager.h"
#include "MeshResourceLoader.h"
#include "AnimatedMeshResourceLoader.h"
#include "MaterialResourceLoader.h"
#include "TextureResourceLoader.h"
#include "SkeletonResourceLoader.h"

#include "WorldSetup.h"
#include "GameClock.h"
#include "VertexBufferManager.h"

#include "GamePropertyManager.h"
#include "AnimationManager.h"

namespace di = boost::di;

inline auto CreateEngineDependencyInjector()
{
	return di::make_injector(
		di::bind<VeritasEngine::IGameClock>().to<VeritasEngine::GameClock>().in(di::singleton),
		di::bind<VeritasEngine::IAnimationManager>().to<VeritasEngine::AnimationManager>().in(di::singleton),
		di::bind<VeritasEngine::IProcessManager>().to<VeritasEngine::ProcessManager>().in(di::singleton),
		di::bind<VeritasEngine::IDeserializeMapping>().to<VeritasEngine::DeserializeMapping>().in(di::singleton),
		di::bind<VeritasEngine::GamePropertyManager>().in(di::singleton),
		di::bind<VeritasEngine::IWorldSetup>().to<VeritasEngine::WorldSetup>().in(di::singleton),
		di::bind<VeritasEngine::IRenderingServices>().to<VeritasEngine::RenderingServices>().in(di::singleton),
		di::bind<VeritasEngine::IScene>().to<VeritasEngine::Scene>().in(di::singleton),
		di::bind<VeritasEngine::IVertexBufferManager>().to<VeritasEngine::VertexBufferManager>().in(di::singleton),
		di::bind<VeritasEngine::IResourceLoader*[]>().to<VeritasEngine::MeshResourceLoader, VeritasEngine::AnimatedMeshResourceLoader, VeritasEngine::MaterialResourceLoader, VeritasEngine::TextureResourceLoader, VeritasEngine::SkeletonResourceLoader>().in(di::singleton),
		di::bind<VeritasEngine::IResourceManager>().to<VeritasEngine::ResourceManager>().in(di::singleton)
	);
}