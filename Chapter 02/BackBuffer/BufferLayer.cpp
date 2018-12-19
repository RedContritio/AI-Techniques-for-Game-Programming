#include "BufferLayer.h"
BUFFERLAYER::BUFFERLAYER(void )
{
	hBufferLayer = NULL ;
	hBitmap = NULL ;
	hPrevBitmap = NULL ;
}

BUFFERLAYER::BUFFERLAYER(HWND hwnd )
{
	BuildAll(hwnd );
}

void BUFFERLAYER::DeleteAll(void )
{
	SelectObject(hBufferLayer ,hPrevBitmap );
	DeleteObject(hBitmap );
	hBitmap = NULL ;
	DeleteDC(hBufferLayer );
	hBufferLayer = NULL ;
}

void BUFFERLAYER::BuildAll(HWND hwnd )
{
	RECT rect ;
	GetClientRect(hwnd ,&rect );
	hBufferLayer = CreateCompatibleDC(NULL );
	HDC hdc = GetDC(hwnd );
	hBitmap = CreateCompatibleBitmap(hdc ,rect.right ,rect.bottom );
	hPrevBitmap = (HBITMAP)SelectObject(hBufferLayer ,hBitmap );
	ReleaseDC(hwnd ,hdc );
}

HDC BUFFERLAYER::GetHdc(void )
{
	return hBufferLayer ;
}

void BUFFERLAYER::AdjustSize(HWND hwnd )
{
	if(hBufferLayer )
	{
		SelectObject(hBufferLayer ,hPrevBitmap );
		DeleteObject(hBitmap );
	}
	RECT rect ;
	GetClientRect(hwnd ,&rect );
	HDC hdc = GetDC(hwnd );
	hBitmap = CreateCompatibleBitmap(hdc ,rect.right ,rect.bottom );
	hPrevBitmap = (HBITMAP)SelectObject(hBufferLayer ,hBitmap );
	ReleaseDC(hwnd ,hdc );
	return ;
}

void BUFFERLAYER::ResetBufferLayer(HWND hwnd )
{
	if(hBufferLayer )DeleteAll( );

	BuildAll(hwnd );
}

void BUFFERLAYER::DeleteBufferLayer(void )
{
	if(hBufferLayer )DeleteAll( );
}
BUFFERLAYER::~BUFFERLAYER(void )
{
	if(hBufferLayer )DeleteAll( );
}