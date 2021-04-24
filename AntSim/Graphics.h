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

	class ImageBuff
	{
		public:
		ImageBuff(uint16_t Width, uint16_t Height)
		{
			width = Width;
			height = Height;
			PixelsPtr = new uint32_t[width * height];
		}

		~ImageBuff()
		{
			delete[] PixelsPtr;
			PixelsPtr = nullptr;
		}

		void resize(uint16_t Width, uint16_t Height)
		{
			width = Width;
			height = Height;
			PixelsPtr = new uint32_t[width * height];
		}

		void PutPix(uint16_t x, uint16_t y, Color col);

		Color GetPix(uint16_t& x, uint16_t& y);

		uint32_t* PixelsPtr = nullptr;	//Pointer for the list of pixels where the picture is being built
		uint16_t width = 0;
		uint16_t height = 0;
	};

	ImageBuff imageBuff = ImageBuff(1, 1);
	RECT resolution;

	bool Init(HWND windowHandle, uint16_t width, uint16_t height);

	void BeginDraw()
	{
		rendertarget->BeginDraw();
	}

	void EndDraw()
	{
		rendertarget->EndDraw();
	}

	void setDrawColor(float r, float g, float b)
	{
		Solidbrush->SetColor(D2D1::ColorF(r, g, b));
	}

	void DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, Color col);

	void refresh();
};
