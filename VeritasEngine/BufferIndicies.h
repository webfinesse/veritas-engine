#ifndef H_BUFFERINDICIES
#define H_BUFFERINDICIES

namespace VeritasEngine
{
	class BufferIndicies
	{
	public:
		BufferIndicies(size_t startIndex, size_t numberOfElements);

		size_t StartIndex;
		size_t NumberOfElements;
	};
}

#endif