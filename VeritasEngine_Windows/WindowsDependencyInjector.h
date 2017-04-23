#pragma once

#include <boost/di.hpp>

namespace di = boost::di;

inline auto CreateWindowsWindowsDependencyInjector()
{
	return di::make_injector(

	);
}
