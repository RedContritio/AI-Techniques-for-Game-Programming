#include "StandardWindow.h"
#include "BufferLayer.h"
#include <cstdlib>
#include <cmath>
const double pi = acos(-1) ;
const char* MainClassName = "BackBufferTestProject" ;
const char* MainTitleName = "Back Buffer" ;
LRESULT CALLBACK WindowProc(HWND hwnd ,
							UINT msg ,
							WPARAM wParam ,
							LPARAM lParam );

int x=100,y=100,r=20,v=1;
int cxClient , cyClient ;
int WINAPI WinMain(HINSTANCE hInstance ,
				   HINSTANCE hPrevInstance ,
				   LPSTR lpCmdLine ,
				   int nShowCmd )
{
	WNDCLASSEX winClass ;
	InitializeWindowClass(&winClass ,WindowProc ,hInstance ,MainClassName );
	if(!RegisterClassEx(&winClass ))
	{
		MessageBox(NULL ,"Window Registration Failed" ,"Error ",0 );
		return 0 ;
	}
	HWND hwnd = CreateVisibleWindow(MainClassName ,MainTitleName ,640 ,480 ,hInstance ) ;

	MSG msg ;
	bool EndFlag = false ;
	while(!EndFlag )
	{
		while(PeekMessage(&msg ,NULL ,0 ,0 ,PM_REMOVE ))
		{
			if(msg.message == WM_QUIT )
			{
				EndFlag = true ;
			}
			else
			{
				TranslateMessage(&msg );
				DispatchMessage(&msg );
			}
		}

		{
			if(x-r<=0 )v = 1 ;
			if(x+r>=cxClient)v = -1 ;
			x += v ;
		}

		InvalidateRect(hwnd ,NULL ,true );
		UpdateWindow(hwnd );
	}
	return 0 ;
}

BUFFERLAYER BackBuffer ;
LRESULT CALLBACK WindowProc(HWND hwnd ,UINT msg ,
							WPARAM wParam ,LPARAM lParam )
{
	switch(msg )
	{
		case WM_CREATE :
		{
			RECT rect ;
			GetWindowRect(hwnd ,&rect );
			cxClient = rect.right ;
			cyClient = rect.bottom ;
			BackBuffer.ResetBufferLayer(hwnd );
			break ;
		}
		case WM_PAINT :
		{
			PAINTSTRUCT ps ;
			BeginPaint(hwnd ,&ps );
			HDC hBackBuffer = BackBuffer.GetHdc( );
			BitBlt(hBackBuffer ,0 ,0 ,cxClient ,cyClient ,NULL ,NULL ,NULL ,WHITENESS );

			Ellipse(hBackBuffer ,x-r ,y-r ,x+r ,y+r );

			BitBlt(ps.hdc ,0 ,0 ,cxClient ,cyClient ,hBackBuffer ,NULL ,NULL ,SRCCOPY );
			EndPaint(hwnd ,&ps );
			break ;
		}
		case WM_SIZE :
		{
			RECT rect ;
			GetWindowRect(hwnd ,&rect );
			cxClient = rect.right ;
			cyClient = rect.bottom ;
			BackBuffer.AdjustSize(hwnd );
			break ;
		}
		case WM_DESTROY :
		{
			BackBuffer.DeleteBufferLayer( );
			PostQuitMessage(0 );
			return 0 ;
		}
		default :
			break ;
	}
	return DefWindowProc(hwnd ,msg ,wParam ,lParam );
}
