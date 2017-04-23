#pragma once

#include <boost/di.hpp>

#include "IndexBufferImpl.h"

namespace di = boost::di;

inline auto CreateWindowsWindowsDependencyInjector()
{
	return di::make_injector(
		di::bind<VeritasEngine::IIndexBuffer>().to<VeritasEngine::IndexBufferImpl>()
	);
}
