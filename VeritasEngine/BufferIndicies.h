#ifndef H_BUFFERINDICIES
#define H_BUFFERINDICIES

#include "SmallObject.h"

namespace VeritasEngine
{
	class BufferIndicies : SmallObject<>
	{
	public:
		BufferIndicies(size_t startIndex, size_t numberOfElements);

		size_t StartIndex;
		size_t NumberOfElements;
	};
}

#endif