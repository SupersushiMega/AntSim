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
			SecPixelsPtr = new uint32_t[width * height];
		}

		~ImageBuff()
		{
			delete[] PixelsPtr;
			PixelsPtr = nullptr;
		}

		void PutPix(uint16_t x, uint16_t y, Color col)
		{
			//Convert Color
			//==========================================================================================================================
			uint32_t buffer = 0;
			buffer |= (uint32_t)(col.b * 255);
			buffer |= ((uint32_t)(col.g * 255) << 8);
			buffer |= ((uint32_t)(col.r * 255) << 16);
			//==========================================================================================================================
			PixelsPtr[x + (y * width)] = buffer;
		}

		Color GetPix(uint16_t& x, uint16_t& y);

		uint32_t* PixelsPtr = nullptr;	//Pointer for the list of pixels where the picture is being built
		uint32_t* SecPixelsPtr = nullptr;	//pointer to the list of pixels which are used in the refresh process
		uint16_t width = 0;
		uint16_t height = 0;
	};

	ImageBuff imageBuff = ImageBuff(1024, 800);
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

	void DrawRect(uint16_t x, uint16_t y, uint16_t width, uint16_t height)
	{
		rendertarget->DrawRectangle(D2D1::RectF(x, y, x + width, y + height), Solidbrush);
	}

	void DrawLine(uint16_t startX, uint16_t startY, uint16_t endX, uint16_t endY)
	{
		rendertarget->DrawLine(D2D1::Point2F(startX, startY), D2D1::Point2F(endX, endY), Solidbrush);
	}

	void DrawCircle(uint16_t x, uint16_t y, uint16_t r)
	{
		rendertarget->DrawEllipse(D2D1::Ellipse(D2D1::Point2(x, y), r, r), Solidbrush);
	}

	void Clear()
	{
		Solidbrush->SetColor(D2D1::ColorF(0.0f, 0.0f, 0.0f));
		rendertarget->FillRectangle(D2D1::RectF(0, 0, resolution.right, resolution.bottom), Solidbrush);
	}
	void ClearPartial(uint16_t xStart, uint16_t xEnd)
	{
		Solidbrush->SetColor(D2D1::ColorF(0.0f, 0.0f, 0.0f));
		rendertarget->FillRectangle(D2D1::RectF(xStart, 0, xEnd, resolution.bottom), Solidbrush);
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
