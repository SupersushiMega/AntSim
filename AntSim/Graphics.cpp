#include "Graphics.h"


Graphics::Graphics()
{
	factory = NULL;
	rendertarget = NULL;
	Solidbrush = NULL;
}


Graphics::~Graphics()
{
	if (factory)
	{
		factory->Release();
	}

	if (rendertarget)
	{
		rendertarget->Release();
	}

	if (Solidbrush)
	{
		Solidbrush->Release();
	}
}


bool Graphics::Init(HWND windowHandle, uint16_t width, uint16_t height)
{
	HRESULT res = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &factory);
	if (res != S_OK)
	{
		return false;
	}

	GetClientRect(windowHandle, &resolution);

	SYSTEM_INFO sysInfoTemp;

	res = factory->CreateHwndRenderTarget
	(
		D2D1::RenderTargetProperties(),
		D2D1::HwndRenderTargetProperties(
			windowHandle,
			D2D1::SizeU(resolution.right, resolution.bottom)),
		&rendertarget
	);

	if (res != S_OK)
	{
		return false;
	}

	rendertarget->SetAntialiasMode(D2D1_ANTIALIAS_MODE_PER_PRIMITIVE);

	res = rendertarget->CreateSolidColorBrush(D2D1::ColorF(0, 0, 0, 0), &Solidbrush);

	if (res != S_OK)
	{
		return false;
	}

	GetSystemInfo(&sysInfoTemp);

	systemInfo = sysInfoTemp;

	imageBuff.resize(width, height);

	return true;
}

void Graphics::ImageBuff::PutPix(uint16_t x, uint16_t y, Color col)
{
	//Convert Color
	//==========================================================================================================================
	uint32_t buffer = 0;
	if (col.b > 0.0f)
	{
		buffer |= (uint32_t)(col.b * 255);
	}

	if (col.g > 0.0f)
	{
		buffer |= ((uint32_t)(col.g * 255) << 8);
	}

	if (col.r > 0.0f)
	{
		buffer |= ((uint32_t)(col.r * 255) << 16);
	}
	//==========================================================================================================================
	PixelsPtr[x + (y * width)] = buffer;
}
