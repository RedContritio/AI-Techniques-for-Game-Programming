#include <windows.h>
#include "BufferLayer.h"
#include "StandardWindow.h"
#include "LogSpawner.h"
#include <cstdio>
#include <cstdlib>
#include <ctime>
#pragma warning( disable:4996)
using RedContritio::BUFFERLAYER ;
using RedContritio::InitializeWindowClass ;
using RedContritio::CreateVisibleWindow ;
using RedContritio::LogSpawner ;

const char* MainClassName = "RedContritio Class" ;
const char* MainTitleName = "RedContritio Standard Window" ;
BUFFERLAYER BackBuffer ;
int cxClient ,cyClient ;
static void GetClientArguments(HWND );

LRESULT CALLBACK WindowProc(HWND ,UINT ,WPARAM ,LPARAM );

int WINAPI WinMain(HINSTANCE hInstance ,HINSTANCE hPrevInstance ,
				   LPSTR lpCmdLine ,int nShowCmd )
{
	LogSpawner loger ; // appropriate constructor is necessary
	if(AllocConsole())
	{
		freopen("CONOUT$","w",stdout);
	}//Open Console
	
	WNDCLASSEX winClass ;
	InitializeWindowClass(&winClass ,WindowProc ,hInstance ,MainClassName );
	if(!RegisterClassEx(&winClass ))
	{
		MessageBox(NULL ,"Window Registration Failed" ,"Error" ,MB_OK );
		return 0 ;
	}
	HWND hwnd = CreateVisibleWindow(MainClassName ,MainTitleName ,640 ,480 ,hInstance );
	if(!hwnd )
	{
		MessageBox(NULL ,"Window Creation Failed" ,"Error" ,MB_OK );
		return 0 ;
	}
	ShowWindow(hwnd ,nShowCmd );
	UpdateWindow(hwnd );

	loger.lprint("Window Creation Succeed" );

	bool EndFlag = false ;
	MSG msg ;
	while(!EndFlag )
	{
		while(PeekMessage(&msg ,NULL ,0 ,0 ,PM_REMOVE )!= 0)
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

		;

		InvalidateRect(hwnd ,NULL ,true );
		UpdateWindow(hwnd );

	}
	
	loger.lprint("Application Quited" );
	return 0 ;
}

LRESULT CALLBACK WindowProc(HWND hwnd ,UINT msg ,WPARAM wParam ,LPARAM lParam )
{
	switch(msg )
	{
		case WM_CREATE :
		{
			srand(time(0 ));
			GetClientArguments(hwnd );
			BackBuffer.ResetBufferLayer(hwnd );
			break ;
		}
		case WM_PAINT :
		{
			PAINTSTRUCT ps ;
			BeginPaint(hwnd ,&ps );
			BitBlt(BackBuffer.GetHdc( ),0 ,0 ,cxClient ,cyClient ,NULL ,NULL ,NULL ,WHITENESS );
			;
			BitBlt(ps.hdc ,0 ,0 ,cxClient ,cyClient ,BackBuffer.GetHdc( ),NULL ,NULL ,SRCCOPY );
			EndPaint(hwnd ,&ps );
			break ;
		}
		case WM_SIZE :
		{
			GetClientArguments(hwnd );
			BackBuffer.AdjustSize(hwnd );
			break ;
		}
		case WM_DESTROY :
		{
			BackBuffer.DeleteBufferLayer( );
			PostQuitMessage(0 );
			break ;
		}
	}
	return DefWindowProc(hwnd ,msg ,wParam ,lParam );
}


static inline void GetClientArguments(HWND hwnd )
{
	RECT rect ;
	GetWindowRect(hwnd ,&rect );
	cxClient = rect.right ;
	cyClient = rect.bottom ;
}