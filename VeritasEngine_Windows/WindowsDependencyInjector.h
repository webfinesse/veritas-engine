#pragma once

#include <../Includes/boost-di/di.hpp>

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
		di::bind<VeritasEngine::DirectXState>().in(di::singleton),
		di::bind<VeritasEngine::IIndexBuffer>().to<VeritasEngine::IndexBufferImpl>().in(di::singleton),
		di::bind<VeritasEngine::IVertexBufferFactory>().to<VeritasEngine::VertexBufferFactory>().in(di::singleton),
		di::bind<VeritasEngine::ITextureLoader>().to<VeritasEngine::TextureResourceLoaderImpl>().in(di::singleton),
		di::bind<VeritasEngine::IMeshShader>().to<VeritasEngine::MeshShaderImpl>().in(di::singleton),
		di::bind<VeritasEngine::IAnimatedMeshShader>().to<VeritasEngine::AnimatedMeshShaderImpl>().in(di::singleton),
		di::bind<VeritasEngine::IRenderer>().to<VeritasEngine::RendererImpl>().in(di::singleton)
	);
}
