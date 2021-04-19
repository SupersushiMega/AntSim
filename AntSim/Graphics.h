#pragma once
using namespace std;

#include <Windows.h>
#include <d2d1.h>
#include <iostream>
#include "Structs.h"

class Graphics
{
	ID2D1Factory* factory;
	ID2D1HwndRenderTarget* rendertarget;
	ID2D1SolidColorBrush* Solidbrush;

	SYSTEM_INFO systemInfo;

public:
	Graphics();
	~Graphics();
	bool Init(HWND windowHandle, uint16_t width, uint16_t height);

	RECT resolution;

	void BeginDraw()
	{
		rendertarget->BeginDraw();
	}

	void EndDraw()
	{
		rendertarget->EndDraw();
	}

	void DrawRect(uint16_t x, uint16_t y, uint16_t width, uint16_t height)
	{
		Solidbrush->SetColor(D2D1::ColorF(1.0f, 0.0f, 0.0f));
		rendertarget->DrawRectangle(D2D1::RectF(x, y, width, height), Solidbrush);
	}
};
