#include <Windows.h>
#include <crtdbg.h>
#include <memory>
#include <tchar.h>

#include "../VeritasEngine/EngineFactory.h"
#include "../VeritasEngine/Engine.h"
#include "../VeritasEngine/IProcessManager.h"
#include "../VeritasEngine/IWorldSetup.h"

#include "../VeritasEngine/IResourceManager.h"
#include "../VeritasEngineBase/ResourceHandle.h"

#include "RotateCameraProcess.h"
#include "RotateObjectProcess.h"

#include <chrono>
#include <iostream>

bool windowResizing = false;
std::unique_ptr<Engine> engine{nullptr};

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if(engine == nullptr)
	{
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}

	switch (uMsg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	case WM_KEYUP:
		if (wParam == VK_SPACE) {
			engine->TogglePause();
			return 0;
		}
		break;
	case WM_ENTERSIZEMOVE:
		engine->SetIsPaused(true);
		windowResizing = true;
		break;
	case WM_EXITSIZEMOVE:
		engine->SetIsPaused(false);
		windowResizing = false;
		break;
	case WM_SIZE:
		if (engine->IsInitialized())
		{
			auto clientWidth = LOWORD(lParam);
			auto clientHeight = HIWORD(lParam);

			if (wParam == SIZE_MINIMIZED)
			{
				engine->SetIsPaused(true);
			}
			else if (wParam == SIZE_MAXIMIZED)
			{
				engine->SetIsPaused(false);
				engine->Reinit(clientWidth, clientHeight);
			}
			else if (wParam == SIZE_RESTORED)
			{
				if (windowResizing == false)
				{
					engine->SetIsPaused(false);
					engine->Reinit(clientWidth, clientHeight);
				}
			}
		}
	}

	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
{
	int tmpDbgFlag = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG);
	tmpDbgFlag |= _CRTDBG_LEAK_CHECK_DF;
	_CrtSetDbgFlag(tmpDbgFlag);

	const wchar_t CLASS_NAME[] = L"Sample Window Class";

	WNDCLASSW wc{};

	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hInstance;
	wc.lpszClassName = CLASS_NAME;
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);

	RegisterClassW(&wc);

	// Create the window.

	unsigned int width = 1024;
	unsigned int height = 768;

	HWND hwnd = CreateWindowExW(
		0,                              // Optional window styles.
		CLASS_NAME,                     // Window class
		nullptr,    // Window text
		WS_OVERLAPPEDWINDOW,            // Window style

		// Size and position
		CW_USEDEFAULT, CW_USEDEFAULT, width, height,

		nullptr,       // Parent window    
		nullptr,       // Menu
		hInstance,  // Instance handle
		nullptr        // Additional application data
		);

	if (hwnd == nullptr)
	{
		return 0;
	}

	ShowWindow(hwnd, nShowCmd);

	auto start = chrono::high_resolution_clock::now();

	engine = std::move(VeritasEngine::CreateEngine());
	engine->Init(hwnd, width, height);

	/*auto objectProcess = std::make_shared<RotateObjectProcess>(2);
	engine.GetProcessManager().AttachProcess(objectProcess);

	objectProcess = std::make_shared<RotateObjectProcess>(3);
	engine.GetProcessManager().AttachProcess(objectProcess);*/

	char wdBuffer[MAX_PATH + 20];
	GetCurrentDirectoryA(sizeof(wdBuffer), wdBuffer);

	std::string workingDirectory(wdBuffer);
	workingDirectory += "\\Resources";

	engine->GetResourceManager().Init(workingDirectory);

	engine->GetWorldSetup().LoadFile("Resources\\\\WorldSetup4.json");
	auto cameraProcess = std::make_shared<RotateCameraProcess>(200.0f, 10s);
	engine->GetProcessManager().AttachProcess(cameraProcess);

	auto end = chrono::high_resolution_clock::now();

	auto startTimeString = std::wstring(L"\r\nStart up time: ");
	auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);
	startTimeString += std::to_wstring(duration.count());
	startTimeString += std::wstring(L" ms");

	OutputDebugString(startTimeString.c_str());

	// Run the message loop.
	MSG msg{};
	unsigned int loopCount = 0;
	while (msg.message != WM_QUIT)
	{
		loopCount++;
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			engine->Loop();
		}

		if(loopCount > 2500)
		{
			wstring newTitle(L"FPS: ");
			newTitle += std::to_wstring(engine->GetCurrentFps());

			SetWindowText(hwnd, newTitle.c_str());
			loopCount = 0;
		}
	}

	engine = nullptr;

	return 0;
}