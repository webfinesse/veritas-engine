#ifndef H_WINDOWS_DEPENDENCY_INJECTOR
#define H_WINDOWS_DEPENDENCY_INJECTOR

#include "../VeritasEngineBase/shared_scope.hpp"

#include "IndexBufferImpl.h"
#include "VertexBufferFactory.h"
#include "TextureResourceLoaderImpl.h"
#include "RendererImpl.h"
#include "MeshShaderImpl.h"
#include "DirectXState.h"
#include "AnimatedMeshShaderImpl.h"

namespace di = boost::di;

inline auto CreateWindowsWindowsDependencyInjector()
{
	return di::make_injector(
		di::bind<VeritasEngine::DirectXState>().in(shared),
		di::bind<VeritasEngine::IIndexBuffer>().to<VeritasEngine::IndexBufferImpl>().in(shared),
		di::bind<VeritasEngine::IVertexBufferFactory>().to<VeritasEngine::VertexBufferFactory>().in(shared),
		di::bind<VeritasEngine::ITextureLoader>().to<VeritasEngine::TextureResourceLoaderImpl>().in(shared),
		di::bind<VeritasEngine::IMeshShader>().to<VeritasEngine::MeshShaderImpl>().in(shared),
		di::bind<VeritasEngine::IAnimatedMeshShader>().to<VeritasEngine::AnimatedMeshShaderImpl>().in(shared),
		di::bind<VeritasEngine::IRenderer>().to<VeritasEngine::RendererImpl>().in(shared)
	);
}

#endif