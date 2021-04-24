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

	void DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, Color col)
	{
		uint16_t buffer = 0;
		float yDelta = 0;
		uint16_t y = 0;
		float yLast = 0;

		if (x1 > x2)	//check if x1 is larger than x2;
		{
			//Swap x1 with x2
			//==========================================================================================================================
			buffer = x1;
			x1 = x2;
			x2 = buffer;
			//==========================================================================================================================

			//Swap y1 with y2
			//==========================================================================================================================
			buffer = y1;
			y1 = y2;
			y2 = buffer;
			//==========================================================================================================================
		}

		yDelta = (float)(y2 - y1) / (x2 - x1);	//calculate slope of line
		
		y = 0;
		yLast = y1;

		for (uint16_t x = x1; x <= x2; x++)
		{
			if (y1 < y2)
			{
				for (y = yLast; y < (yLast + yDelta); y++)
				{
					imageBuff.PutPix(x, y, col);
				}
			}
			else if (y1 > y2)
			{
				if (yLast == y2)	//Check if yLast is equal to y2 to ensure no crashes happen
				{
					imageBuff.PutPix(x, yLast, col);
				}
				else
				{
					for (y = yLast; y >= (yLast + yDelta); y--)
					{
						imageBuff.PutPix(x, y, col);
					}
					imageBuff.PutPix(x, y, col);
				}
			}
			else
			{
				y = yLast;
				imageBuff.PutPix(x, y, col);
			}
			yLast += yDelta;
		}
	}

	void refresh()
	{
		ID2D1Bitmap* BufferBmp;
	
		//Create Bitmap
		//==========================================================================================================================
		D2D1_PIXEL_FORMAT PixForm = D2D1::PixelFormat();
		PixForm.format = DXGI_FORMAT_B8G8R8A8_UNORM_SRGB;
		PixForm.alphaMode = D2D1_ALPHA_MODE_IGNORE;
	
		D2D1_BITMAP_PROPERTIES Properties = D2D1::BitmapProperties();
		rendertarget->GetDpi(&Properties.dpiX, &Properties.dpiY);
		Properties.pixelFormat = PixForm;
	
		D2D1_RECT_F rect = D2D1::RectF(0.0f, 0.0f, imageBuff.width, imageBuff.height);
		D2D1_SIZE_U size = D2D1::SizeU(imageBuff.width, imageBuff.height);
		HRESULT hr = rendertarget->CreateBitmap(size, imageBuff.PixelsPtr, imageBuff.width * 4, Properties, &BufferBmp);
		//==========================================================================================================================
	
		//Copy Buffer to Bitmap
		//==========================================================================================================================
		D2D1_RECT_U CopyRect = D2D1::RectU(0, 0, imageBuff.width, imageBuff.height);
	
		BufferBmp->CopyFromMemory(&CopyRect, imageBuff.PixelsPtr, imageBuff.width * 4);
		//==========================================================================================================================
	
		rendertarget->BeginDraw();
		//Draw Bitmap
		//==========================================================================================================================
		rendertarget->DrawBitmap(BufferBmp, rect);
		//==========================================================================================================================
		BufferBmp->Release();
	
		rendertarget->EndDraw();
	}
};
