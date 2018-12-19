#include <windows.h>
#include "BufferLayer.h"
#include "StandardWindow.h"
#include "LogSpawner.h"
#include "CTimer.h"
#include <cstdio>
#include <cstdlib>
#include <ctime>

#include "S2DMatrix.h"
#include "S2DVector.h"

#include "CController.h"
#include "CStar.h"

using RedContritio::BUFFERLAYER ;
using RedContritio::LOGSPAWNER ;
using RedContritio::InitializeWindowClass ;
using RedContritio::CreateVisibleWindow ;

const char* MainClassName = "RedContritio Test MB Project" ;
const char* MainTitleName = "Mutiply Bodys" ;
BUFFERLAYER BackBuffer ;
int cxClient ,cyClient ;
int pxMouse ,pyMouse ;
static void GetClientArguments(HWND );

LOGSPAWNER loger ;

CONTROLLER* pController = NULL ;

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
	HWND hwnd = CreateVisibleWindow(MainClassName ,MainTitleName ,640 ,480 ,hInstance );
	if(!hwnd )
	{
		MessageBox(NULL ,"Window Creation Failed" ,"Error" ,MB_OK );
		return 0 ;
	}
	ShowWindow(hwnd ,nShowCmd );
	UpdateWindow(hwnd );

	loger.lprint("Window Creation Succeeded\n" );

	bool EndFlag = false ;
#ifndef FRAMES__PER__SECOND
	#define FRAMES__PER__SECOND 60
#endif
	CTimer timer(FRAMES__PER__SECOND );
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
			if(pController )
			{
				pController->Update(timer.TimeElapsed());
			}
			InvalidateRect(hwnd, NULL, true);
			UpdateWindow(hwnd);
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
			srand((unsigned)time(NULL ));
			GetClientArguments(hwnd );
			BackBuffer.ResetBufferLayer(hwnd );
			pController = new CONTROLLER();
			break ;
		}
		case WM_PAINT :
		{
			PAINTSTRUCT ps ;
			BeginPaint(hwnd ,&ps );

			BitBlt(BackBuffer.GetHdc( ),0 ,0 ,cxClient ,cyClient ,NULL ,0 ,0 ,BLACKNESS );

			if(pController )
			{
				pController->Render(BackBuffer.GetHdc());
			}

			BitBlt(ps.hdc ,0 ,0 ,cxClient ,cyClient ,BackBuffer.GetHdc( ),0 ,0 ,SRCCOPY );

			EndPaint(hwnd ,&ps );
			break ;
		}
		case WM_KEYUP :
		{
			switch(wParam )
			{
				case 'R' :
				{
					delete pController ;
					pController = new CONTROLLER() ;
					break ;
				}
				case 'V' :
				{
					pController->Watch();
					break ;
				}
				case 'P' :
				{
					if(pController )
					{
						pController->Pause();
					}
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
					SendMessage(hwnd ,WM_DESTROY ,NULL ,NULL );
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
		case WM_LBUTTONDOWN :
		{
			int x = LOWORD(lParam );
			int y = HIWORD(lParam );
			if(pController )
			{
				pController->LButtonDown(x ,y );
			}
			break ;
		}
		case WM_LBUTTONUP :
		{
			int x = LOWORD(lParam );
			int y = HIWORD(lParam );
			if(pController )
			{
				pController->LButtonUp(x ,y );
			}
			break ;
		}
		case WM_RBUTTONDOWN :
		{
			int x = LOWORD(lParam);
			int y = HIWORD(lParam);
			if(pController )
			{
				pController->RButtonDown(x ,y );
			}
			break ;
		}
		case WM_RBUTTONUP :
		{
			int x = LOWORD(lParam);
			int y = HIWORD(lParam);
			if(pController )
			{
				pController->RButtonUp(x ,y );
			}
			break ;
		}
		case WM_MOUSEMOVE :
		{
			pxMouse = LOWORD(lParam );
			pyMouse = HIWORD(lParam );
			if(pController )
			{
				pController->MouseMove(pxMouse ,pyMouse );
			}
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