#ifndef H_FILE_HELPER
#define H_FILE_HELPER

#include <string>
#include <unordered_map>

namespace VeritasEngine
{
	class FileHelper
	{
	public:
		static unsigned char* ReadFile(const char* filename, unsigned int* fileLength);
		static std::unordered_map<std::string, std::string> FindAllResourceFilesInDirectory(const std::string& fullPath);
	};
}

#endif