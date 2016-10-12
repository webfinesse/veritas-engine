#ifndef H_WORLDSETUP
#define H_WORLDSETUP

#include "DynamicLibraryHelper.h"

namespace VeritasEngine
{
	class EXPORT WorldSetup
	{
	public:
		static void LoadFile(const char* filename);
	};
}

#endif