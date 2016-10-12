#ifndef H_WINUTILS
#define H_WINUTILS

#include <Windows.h>
#include <string>

using namespace std;

#define HHR(x, y) VeritasEngine::WindowsUtil::HandleHResult(x, y, __FILE__, __LINE__)

namespace VeritasEngine
{
	class WindowsUtil
	{
	public:
		static void HandleHResult(const HRESULT& hr, const char* message, const char* file, unsigned int line)
		{
			if (FAILED(hr))
			{
				char buffer[2048];

				sprintf_s(buffer, sizeof(buffer) - 1, "HResult Failure: %s\r\nFile: %s\r\nLine: %d", message, file, line);

				MessageBoxA(nullptr, buffer, "Invalid HResult", MB_OK);
			}
		}
	};
}

#endif