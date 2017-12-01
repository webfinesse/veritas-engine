#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <crtdbg.h>
#include <chrono>

#include "../VeritasEngine/EngineFactory.h"
#include "../VeritasEngine/Engine.h"
#include "../VeritasEngine/IProcessManager.h"
#include "../VeritasEngine/IWorldSetup.h"
#include "../VeritasEngine/IAnimationManager.h"
#include "../VeritasEngine/IResourceManager.h"
#include "../VeritasEngine/GameObjectPropertyKeys.h"
#include "../VeritasEngine/GamePropertyManager.h"

#include "../VeritasEngineBase/StringHelper.h"

#include "RotateCameraProcess.h"
#include "../VeritasEngine/MatrixStack.h"

bool windowResizing = false;
constexpr unsigned int width = 1024;
constexpr unsigned int height = 768;

std::unique_ptr<VeritasEngine::Engine> engine{nullptr};

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

HWND CreateGameWindow(_In_ HINSTANCE hInstance)
{
#if _DEBUG
	int tmpDbgFlag = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG);
	tmpDbgFlag |= _CRTDBG_LEAK_CHECK_DF;
	_CrtSetDbgFlag(tmpDbgFlag);
#endif

	const wchar_t CLASS_NAME[] = L"VeritasEngine";

	WNDCLASSW wc{};

	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hInstance;
	wc.lpszClassName = CLASS_NAME;
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);

	RegisterClassW(&wc);

	// Create the window.
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

	return hwnd;
}

void InitializeEngine(const HWND hwnd)
{
	const auto start = std::chrono::high_resolution_clock::now();

	engine = std::move(VeritasEngine::CreateEngine());
	engine->Init(hwnd, width, height);

	char wdBuffer[MAX_PATH + 20];
	GetCurrentDirectoryA(sizeof(wdBuffer), wdBuffer);

	engine->GetResourceManager().Init(VeritasEngine::FormatString("%s\\Resources", wdBuffer));

	engine->GetWorldSetup().LoadFile("Resources\\WorldSetup4.json");

	const auto cameraProcess = std::make_shared<RotateCameraProcess>(engine->GetGamePropertyManager(), 500.0f, std::chrono::seconds(10));
	engine->GetProcessManager().AttachProcess(cameraProcess);

	const auto end = std::chrono::high_resolution_clock::now();

	const auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
	const auto startTimeString = VeritasEngine::FormatString(L"\r\nStart up time: %d ms\r\n", duration.count());

	OutputDebugString(startTimeString.c_str());
}

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
{
	const auto hwnd = CreateGameWindow(hInstance);

	InitializeEngine(hwnd);

	ShowWindow(hwnd, nShowCmd);

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
			const auto newTitle = VeritasEngine::FormatString(L"FPS: %.2f", engine->GetCurrentFps());

			SetWindowText(hwnd, newTitle.c_str());
			loopCount = 0;
		}
	}

	engine = nullptr;

	return 0;
}
