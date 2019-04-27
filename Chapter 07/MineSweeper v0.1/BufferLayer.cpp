#include "BufferLayer.h"
namespace RedContritio
{

#pragma warning(push)
#pragma warning(disable:6387)
BUFFERLAYER::BUFFERLAYER(void) : hdc(NULL), hBitmap(NULL), hPrevBitmap(NULL)
{
}

BUFFERLAYER::BUFFERLAYER(HWND hwnd) : hdc(NULL), hBitmap(NULL), hPrevBitmap(NULL)
{
	build(hwnd);
}

void BUFFERLAYER::release(void)
{
	SelectObject(hdc, hPrevBitmap);
	DeleteObject(hBitmap);
	hBitmap = NULL;
	DeleteDC(hdc);
	hdc = NULL;
}

void BUFFERLAYER::build(HWND hwnd)
{
	RECT rect;
	GetClientRect(hwnd, &rect);
	hdc = CreateCompatibleDC(NULL);
	HDC _hdc = GetDC(hwnd);
	hBitmap = CreateCompatibleBitmap(_hdc, rect.right, rect.bottom);
	hPrevBitmap = (HBITMAP) SelectObject(hdc, hBitmap);
	ReleaseDC(hwnd, _hdc);
}

void BUFFERLAYER::Adjust(HWND hwnd)
{
	if ( hdc )
	{
		SelectObject(hdc, hPrevBitmap);
		DeleteObject(hBitmap);
	}
	RECT rect;
	GetClientRect(hwnd, &rect);
	HDC _hdc = GetDC(hwnd);
	hBitmap = CreateCompatibleBitmap(_hdc, rect.right, rect.bottom);
	hPrevBitmap = (HBITMAP) SelectObject(hdc, hBitmap);
	ReleaseDC(hwnd, _hdc);
	return;
}

void BUFFERLAYER::Reset(HWND hwnd)
{
	if ( hdc )release();

	build(hwnd);
}

void BUFFERLAYER::Delete(void)
{
	if ( hdc )release();
}
BUFFERLAYER::~BUFFERLAYER(void)
{
	if ( hdc )release();
}

#pragma warning(pop)
}
