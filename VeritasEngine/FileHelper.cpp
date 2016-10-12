#include "FileHelper.h"

#include <fstream>

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