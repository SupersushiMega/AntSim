#define WIDTH 1024
#define HEIGHT 800

#include <Windows.h>
#include "Graphics.h"
#include "AntSim.h"
#include <algorithm>
#include <time.h>


Graphics* graphics;
bool closeWindow = false;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (uMsg == WM_DESTROY)
	{
		PostQuitMessage(0);
		closeWindow = true;
		return 0;
	}

	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE prevInstance, LPWSTR cmd, int nCmdShow)
{
	RECT resolution = { 0, 0, WIDTH, HEIGHT };

	//Window Setup
	//=========================================================================================
	WNDCLASSEX windowclass;
	ZeroMemory(&windowclass, sizeof(WNDCLASSEX));

	windowclass.cbSize = sizeof(WNDCLASSEX);
	windowclass.hbrBackground = (HBRUSH)COLOR_WINDOW;
	windowclass.hInstance = hInstance;
	windowclass.lpfnWndProc = WindowProc;
	windowclass.lpszClassName = L"MainWindow";
	windowclass.style = CS_HREDRAW | CS_VREDRAW;

	RegisterClassEx(&windowclass);

	RECT rect = resolution; //Drawing area
	AdjustWindowRectEx(&rect, WS_OVERLAPPEDWINDOW, false, WS_EX_OVERLAPPEDWINDOW);	//Calculate window Size

	HWND windowhandle = CreateWindowEx(WS_EX_OVERLAPPEDWINDOW, L"MainWindow", L"AntSim", WS_OVERLAPPEDWINDOW, 100, 100, rect.right - rect.left, rect.bottom - rect.top, NULL, NULL, hInstance, 0);
	//=========================================================================================

	if (!windowhandle)
	{
		return -1;
	}

	graphics = new Graphics();

	MSG message;
	message.message = WM_NULL;

	if (!graphics->Init(windowhandle, resolution.right, resolution.bottom))
	{
		delete graphics;
		return -1;
	}

	ShowWindow(windowhandle, nCmdShow);

	Colony colo = Colony(graphics);

	colo.MakeTileMap(1024, 800);

	for (uint32_t i = 0; i < 2000; i++)
	{
		colo.addAnt();
	}

	uint8_t frame = 0;

	Color col = { 1.0f, 1.0f, 1.0f };

	while (!closeWindow)
	{
		frame++;
		if (PeekMessage(&message, windowhandle, 0, 0, PM_REMOVE))
		{
			DispatchMessage(&message);
		}
		colo.simulateStep();
		if (frame % 4)
		{
			graphics->BeginDraw();
			//graphics->Clear();
			colo.drawTileMap();
			graphics->DrawLine(0, 128, 1, frame, col);
			//colo.drawAnts();
			graphics->EndDraw();
			graphics->refresh();
		}
	}

	delete graphics;

	return 0;
}
