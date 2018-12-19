#include <cstdlib>
#include <ctime>
#include "StandardWindow.h"
#include "BufferLayer.h"
#include "Population.h"
#include "CTimer.h"
#include "utils.h"
#pragma warning( disable:4996)
/*
	Unfinised Projects
*/
using RedContritio::BUFFERLAYER ;
using RedContritio::InitializeWindowClass ;
using RedContritio::CreateVisibleWindow ;

const char* MainClassName = "Basic AI Learing Project" ;
const char* MainTitleName = "Labyrinth" ;

int cxClient ,cyClient ;
BUFFERLAYER BackBuffer ;

POPULATION *Population ;

LRESULT CALLBACK WindowProc(HWND hwnd ,UINT msg ,
							WPARAM wParam ,LPARAM lParam );

int WINAPI WinMain(HINSTANCE hInstance ,HINSTANCE hPrevInstance ,
				   LPSTR lpCmdLine ,int nShowCmd )
{
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

	bool EndFlag = false ;
	MSG msg ;
	while(!EndFlag )
	{
		while(PeekMessage(&msg ,NULL ,0 ,0 ,PM_REMOVE )!=0 )
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

		if(Population->Started() )
		{
			Population->Epoch( );
			InvalidateRect(hwnd ,NULL ,true );
			UpdateWindow(hwnd );
		}
	}

	UnregisterClass(MainClassName ,winClass.hInstance );
	return 0 ;
}

LRESULT CALLBACK WindowProc(HWND hwnd ,UINT msg ,
							WPARAM wParam ,LPARAM lParam )
{
	switch(msg )
	{
		case WM_CREATE :
		{
			srand(( unsigned)time(NULL ));
			RECT rect ;
			GetWindowRect(hwnd ,&rect );
			cxClient = rect.right ;
			cyClient = rect.bottom ;

			BackBuffer.ResetBufferLayer(hwnd );
			
			Population = new POPULATION(__RATE__CROSSRATE ,
										__RATE__MUTATION ,
										__SIZE__POP ,
										__LENGTH__CHROMO ,
										__LENGTH__GENE );
			break ;
		}
		case WM_PAINT :
		{
			PAINTSTRUCT ps ;
			BeginPaint(hwnd ,&ps );
			BitBlt(BackBuffer.GetHdc() ,0 ,0 ,cxClient ,cyClient ,
				   NULL ,NULL ,NULL ,WHITENESS );

			Population->Render(cxClient ,cyClient ,BackBuffer.GetHdc() );

			BitBlt(ps.hdc ,0 ,0 ,cxClient ,cyClient ,
				   BackBuffer.GetHdc() ,NULL ,NULL ,SRCCOPY );
			EndPaint(hwnd ,&ps );
			break ;
		}
		case WM_KEYUP :
		{
			switch(wParam )
			{
				case VK_RETURN :
				{
					Population->Run(hwnd );
					break ;
				}
				case VK_SPACE :
				{
					Population->Stop() ;
					break ;
				}
				default :
					break ;
			}
			break ;
		}
		case WM_SIZE :
		{
			cxClient = LOWORD(lParam );
			cyClient = HIWORD(lParam );

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