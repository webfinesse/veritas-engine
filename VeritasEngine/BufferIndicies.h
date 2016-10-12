#ifndef H_BUFFERINDICIES
#define H_BUFFERINDICIES

#include "../VeritasEngineBase/SmallObject.h"

namespace VeritasEngine
{
	class BufferIndicies : SmallObject<>
	{
	public:
		BufferIndicies(unsigned int startIndex, unsigned int numberOfElements);

		unsigned int StartIndex;
		unsigned int NumberOfElements;
	};
}

#endif