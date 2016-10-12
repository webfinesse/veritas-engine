#include "FileHelper.h"

#include <fstream>

#include <boost/filesystem.hpp>

namespace fs = boost::filesystem;

using namespace std;

unsigned char* VeritasEngine::FileHelper::ReadFile(const char* filename, unsigned int* fileLength)
{
	std::basic_ifstream<unsigned char> inFile;
	unsigned int size = 0;

	inFile.open(filename, ios::in | ios::binary);

	if (inFile.is_open())
	{
		inFile.seekg(0, ios::end);
		size = static_cast<unsigned int>(inFile.tellg());
		inFile.seekg(0, ios::beg);

		unsigned char* result = new unsigned char[size];
		inFile.read(result, size);

		*fileLength = size;

		return result;
	}

	return nullptr;
}

std::unordered_map<std::string, std::string> VeritasEngine::FileHelper::FindAllResourceFilesInDirectory(const std::string& fullPath)
{
	auto result = std::unordered_map<std::string, std::string>();

	auto path = fs::path(fullPath);
	fs::path resourceFileExt(".zip");

	if (fs::is_directory(fullPath))
	{
		for (auto& item : fs::directory_iterator(fullPath))
		{
			auto& itemPath = item.path();
			if (fs::is_regular_file(itemPath))
			{
				if (itemPath.has_extension() && itemPath.extension() == resourceFileExt)
				{
					result[itemPath.filename().replace_extension("").generic_string()] = itemPath.generic_string();
				}
			}
		}
	}

	return result;
}
