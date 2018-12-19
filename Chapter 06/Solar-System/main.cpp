#include <windows.h>
#include "BufferLayer.h"
#include "StandardWindow.h"
#include "LogSpawner.h"
#include "CTimer.h"
#include "resource.h"
#include <cstdio>
#include <cstdlib>
#include <ctime>

#include "S2DMatrix.h"
#include "S2DVector.h"

#include "CSolar.h"

using RedContritio::BUFFERLAYER ;
using RedContritio::InitializeWindowClass ;
using RedContritio::CreateVisibleWindow ;
using RedContritio::LogSpawner ;

const char* MainClassName = "RedContritio Test SS Project" ;
const char* MainTitleName = "Solar System" ;
BUFFERLAYER BackBuffer ;
int cxClient ,cyClient ;
int pxMouse ,pyMouse ;
static void GetClientArguments(HWND );
#ifdef DEBUG
LogSpawner loger ;
#endif

SOLAR* pSolar ;

LRESULT CALLBACK WindowProc(HWND ,UINT ,WPARAM ,LPARAM );

int WINAPI WinMain(HINSTANCE hInstance ,HINSTANCE hPrevInstance ,
				   LPSTR lpCmdLine ,int nShowCmd )
{
#ifdef DEBUG
	loger.lprint("Application Started\n" );
#endif
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
	winClass.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1 );
	winClass.hIconSm = LoadIcon(hInstance ,MAKEINTRESOURCE(IDI_ICON1 ));
	winClass.hIcon = LoadIcon(hInstance ,MAKEINTRESOURCE(IDI_ICON1 ));
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
#ifdef DEBUG
	loger.lprint("Window Creation Succeeded\n" );
#endif

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
			if(pSolar )
			{
				pSolar->Update(timer.GetTimeElapsed() );
			}
			InvalidateRect(hwnd, NULL, true);
			UpdateWindow(hwnd);
		}

	}

	UnregisterClass(MainClassName ,winClass.hInstance );
#ifdef DEBUG
	loger.lprint("Application Quited\n" );
#endif
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
			pSolar = new SOLAR( );
			//PlaySound("BGM", NULL, SND_ASYNC|SND_FILENAME|SND_LOOP );
			break ;
		}
		case WM_PAINT :
		{
			PAINTSTRUCT ps ;
			BeginPaint(hwnd ,&ps );

			BitBlt(BackBuffer.GetHdc( ),0 ,0 ,cxClient ,cyClient ,NULL ,0 ,0 ,BLACKNESS );

			pSolar->Render(BackBuffer.GetHdc());// Render

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
					if(pSolar )delete pSolar ;
					pSolar = new SOLAR() ;
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
			pSolar->ReadyToAddPlanet(x ,y );
			break ;
		}
		case WM_LBUTTONUP :
		{
			int x = LOWORD(lParam );
			int y = HIWORD(lParam );
			pSolar->AddPlanet(x ,y );
			break ;
		}
		case WM_RBUTTONDOWN :
		{
			int x = LOWORD(lParam);
			int y = HIWORD(lParam);
			pSolar->AddOrbit(x, y);
			break ;
		}
		case WM_MOUSEMOVE :
		{
			pxMouse = LOWORD(lParam );
			pyMouse = HIWORD(lParam );
			break ;
		}
		case WM_COMMAND :
		{
			switch(wParam )
			{
				case ID_FILE_RESTART :
				{
					if(pSolar )delete pSolar ;
					pSolar = new SOLAR ;
					break ;
				}
				case ID_FILE_EXIT :
				{
					PostQuitMessage(0 );
					break ;
				}
				case ID_ABOUT :
				{
					MessageBox(NULL ,"Made By RedContritio" ,"Informations" ,MB_OK );
					break ;
				}
			}
			break ;
		}
		case WM_DESTROY :
		{
			BackBuffer.DeleteBufferLayer( );
			PostQuitMessage(0 );
			if(pSolar )
			{
				delete pSolar ;
			}
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