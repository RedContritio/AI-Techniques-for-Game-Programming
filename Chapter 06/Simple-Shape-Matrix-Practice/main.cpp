#include <windows.h>
#include "BufferLayer.h"
#include "StandardWindow.h"
#include "LogSpawner.h"
#include "CTimer.h"
#include <cstdio>
#include <cstdlib>
#include <ctime>

#include "utils.h"
#include "defines.h"
#include "Gun.h"

using RedContritio::BUFFERLAYER ;
using RedContritio::InitializeWindowClass ;
using RedContritio::CreateVisibleWindow ;
using RedContritio::LogSpawner ;

const char* MainClassName = "RedContritio Test SSMP Practice" ;
const char* MainTitleName = "Simple Shape Matrix Practice" ;
BUFFERLAYER BackBuffer ;
int cxClient ,cyClient ;
static void GetClientArguments(HWND );

LogSpawner loger ;

GUN gun(__WINDOW__WIDTH / 2 ,__WINDOW__HEIGHT / 2 ,5 , 0 );

LRESULT CALLBACK WindowProc(HWND ,UINT ,WPARAM ,LPARAM );

int WINAPI WinMain(HINSTANCE hInstance ,HINSTANCE hPrevInstance ,
				   LPSTR lpCmdLine ,int nShowCmd )
{
	loger.lprint("Application Started\n" );
	/*
	{
		time_t seed = time(NULL );
		srand((unsigned int)seed );
		loger.lprint("seed = %d\n",seed );
	}
	*/
	/*
	if(AllocConsole())
	{
		freopen("CONOUT$","w",stdout);
	}//Open Console
	*/

	WNDCLASSEX winClass ;
	InitializeWindowClass(&winClass ,WindowProc ,hInstance ,MainClassName );
	if(!RegisterClassEx(&winClass ))
	{
		MessageBox(NULL ,"Window Registration Failed" ,"Error" ,MB_OK );
		return 0 ;
	}
	HWND hwnd = CreateVisibleWindow(MainClassName ,MainTitleName ,__WINDOW__WIDTH ,__WINDOW__HEIGHT ,hInstance );
	if(!hwnd )
	{
		MessageBox(NULL ,"Window Creation Failed" ,"Error" ,MB_OK );
		return 0 ;
	}
	ShowWindow(hwnd ,nShowCmd );
	UpdateWindow(hwnd );

	loger.lprint("Window Creation Succeeded\n" );

	bool EndFlag = false ;
	CTimer timer(__FRAME__PER__SECOND );
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

		
		if(timer.ReadyForNextFrame() )
		{
			gun.Update( );
			InvalidateRect(hwnd ,NULL ,true );
			UpdateWindow(hwnd );
		}

	}

	UnregisterClass(MainClassName ,winClass.hInstance );

	loger.lprint("Application Quited\n" );
	return 0 ;
}

LRESULT CALLBACK WindowProc(HWND hwnd ,UINT msg ,WPARAM wParam ,LPARAM lParam )
{
	switch(msg )
	{
		case WM_CREATE :
		{
			GetClientArguments(hwnd );
			BackBuffer.ResetBufferLayer(hwnd );
			break ;
		}
		case WM_PAINT :
		{
			PAINTSTRUCT ps ;
			BeginPaint(hwnd ,&ps );

			BitBlt(BackBuffer.GetHdc( ),0 ,0 ,cxClient ,cyClient ,NULL ,NULL ,NULL ,WHITENESS );

			gun.Render(BackBuffer.GetHdc());

			BitBlt(ps.hdc ,0 ,0 ,cxClient ,cyClient ,BackBuffer.GetHdc( ),NULL ,NULL ,SRCCOPY );

			EndPaint(hwnd ,&ps );
			break ;
		}
		case WM_KEYUP :
		{
			
			switch(wParam )
			{
				case 'S' :
				{
					//pPopulation->SlowMode = true ;
					break ;
				}
				case 'F' :
				{
					//pPopulation->SlowMode = false ;
					break ;
				}
				case VK_RETURN :
				{
					break ;
				}
				case VK_SPACE :
				{
					break ;
				}
				case VK_LEFT : case VK_UP :
				case VK_RIGHT : case VK_DOWN :
				{
					break ;
				}
				case VK_ESCAPE :
				{
					PostQuitMessage(0 );
					break ;
				}
			}
			break ;
		}
		case WM_SIZE :
		{
			GetClientArguments(hwnd );
			BackBuffer.AdjustSize(hwnd );
			break ;
		}
		case WM_COMMAND :
		{
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
	GetClientRect(hwnd ,&rect );
	cxClient = rect.right ;
	cyClient = rect.bottom ;
}