#ifndef H_BUFFERINDICIES
#define H_BUFFERINDICIES

namespace VeritasEngine
{
	class BufferIndicies
	{
	public:
		BufferIndicies(const size_t startIndex, const size_t numberOfElements);

		size_t StartIndex;
		size_t NumberOfElements;
	};
}

#endif