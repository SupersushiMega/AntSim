#define WIDTH 1024
#define HEIGHT 800

#include <Windows.h>
#include <algorithm>
#include <time.h>
#include <string>
#include "Graphics.h"
#include "AntSim.h"
#include "Perlin.h"


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

	Colony colo = Colony(graphics, 512, 400);

	colo.MakeTileMap(1024, 800);

	uint8_t frame = 0;

	Color col = { 0.0f, 0.0f, 0.0f };

	Perlin2D perlin(1024, 800);
	perlin.generateNoise(24, 1.4f);

	for (uint16_t x = 0; x < 1024; x++)
	{
		for (uint16_t y = 0; y < 800; y++)
		{
			col.r = perlin.noise[x + (y * perlin.width)];
			graphics->imageBuff.PutPix(x, y, col);
		}
	}
	graphics->refresh();
	for (uint16_t x = 0; x < 1024; x++)
	{
		for (uint16_t y = 0; y < 800; y++)
		{
			float value = perlin.noise[x + (y * perlin.width)];
			if(value > 0.4f)
			{
				tile temp;
				temp = colo.tileMap.ReadMap_WC(x, y);
				temp.type = FOOD;
				colo.tileMap.WriteToMap_WC(x, y, temp);
			}
		}
	}
	uint32_t i = 0;
	for (i = 0; i < 100; i++)
	{
		colo.addAnt();
	}

	uint32_t temp;

	graphics->refresh();

	while (!closeWindow)
	{
		frame++;
		if (PeekMessage(&message, windowhandle, 0, 0, PM_REMOVE))
		{
			DispatchMessage(&message);
		}
		colo.simulateStep();
		temp = colo.Ants.capacity();
		if (1)
		{
			//graphics->Clear();
			colo.drawTileMap();
			colo.drawAnts();
			graphics->refresh();
		}
	}

	delete graphics;

	return 0;
}
