#ifndef H_FILE_HELPER
#define H_FILE_HELPER

namespace VeritasEngine
{
	class FileHelper
	{
	public:
		static unsigned char* ReadFile(const char* filename, unsigned int* fileLength);
	};
}

#endif