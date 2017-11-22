#ifndef H_ENGINE_DEPENDENCY_INJECTOR
#define H_ENGINE_DEPENDENCY_INJECTOR

#include "../VeritasEngineBase/shared_scope.hpp"

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
		di::bind<VeritasEngine::IGameClock>().to<VeritasEngine::GameClock>().in(shared),
		di::bind<VeritasEngine::IAnimationManager>().to<VeritasEngine::AnimationManager>().in(shared),
		di::bind<VeritasEngine::IProcessManager>().to<VeritasEngine::ProcessManager>().in(shared),
		di::bind<VeritasEngine::IDeserializeMapping>().to<VeritasEngine::DeserializeMapping>().in(shared),
		di::bind<VeritasEngine::GamePropertyManager>().in(shared),
		di::bind<VeritasEngine::IWorldSetup>().to<VeritasEngine::WorldSetup>().in(shared),
		di::bind<VeritasEngine::IRenderingServices>().to<VeritasEngine::RenderingServices>().in(shared),
		di::bind<VeritasEngine::IScene>().to<VeritasEngine::Scene>().in(shared),
		di::bind<VeritasEngine::IVertexBufferManager>().to<VeritasEngine::VertexBufferManager>().in(shared),
		di::bind<VeritasEngine::IResourceLoader*[]>().to<VeritasEngine::MeshResourceLoader, VeritasEngine::AnimatedMeshResourceLoader, VeritasEngine::MaterialResourceLoader, VeritasEngine::TextureResourceLoader, VeritasEngine::SkeletonResourceLoader>(),
		di::bind<VeritasEngine::IResourceManager>().to<VeritasEngine::ResourceManager>().in(shared)
	);
}

#endif