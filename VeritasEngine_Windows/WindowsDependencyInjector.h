#pragma once

#include <boost/di.hpp>

#include "IndexBufferImpl.h"
#include "VertexBufferFactory.h"
#include "RendererImpl.h"

namespace di = boost::di;

inline auto CreateWindowsWindowsDependencyInjector()
{
	return di::make_injector(
		di::bind<VeritasEngine::IIndexBuffer>().to<VeritasEngine::IndexBufferImpl>(),
		di::bind<VeritasEngine::IVertexBufferFactory>().to<VeritasEngine::VertexBufferFactory>(),
		di::bind<VeritasEngine::IRenderer>().to<VeritasEngine::RendererImpl>()
	);
}
