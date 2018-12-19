#include <windows.h>
#include "BufferLayer.h"
#include "StandardWindow.h"
#include "Map.h"
#include "Genome.h"
#include "utils.h"
#include <cstdlib>
#include <ctime>

using RedContritio::BUFFERLAYER ;
using RedContritio::InitializeWindowClass ;
using RedContritio::CreateVisibleWindow ;

// I don't know how to solve it, so I gave up this project

const char* MainClassName = "TSP" ;
const char* MainTitleName = "Traveling Salesman Problem" ;
BUFFERLAYER BackBuffer ;
int cxClient ,cyClient ;
static void GetClientArguments(HWND );

POPULATION *salemen ;
LRESULT CALLBACK WindowProc(HWND ,UINT ,WPARAM ,LPARAM );

int WINAPI WinMain(HINSTANCE hInstance ,HINSTANCE hPrevInstance ,
				   LPSTR lpCmdLine ,int nShowCmd )
{
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
	if(salemen )delete salemen ;
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

			salemen = new POPULATION(__MUTATION__RATE ,__CROSSOVER__RATE ,
									 __POPULATION__SIZE ,__CITY__NUM ,
									 640 ,480 );
			break ;
		}
		case WM_PAINT :
		{
			PAINTSTRUCT ps ;
			BeginPaint(hwnd ,&ps );
			BitBlt(BackBuffer.GetHdc( ),0 ,0 ,cxClient ,cyClient ,NULL ,NULL ,NULL ,WHITENESS );
			salemen->Render(BackBuffer.GetHdc( ),cxClient ,cyClient );
			BitBlt(ps.hdc ,0 ,0 ,cxClient ,cyClient ,BackBuffer.GetHdc( ),NULL ,NULL ,SRCCOPY );
			EndPaint(hwnd ,&ps );
			break ;
		}
		case WM_SIZE :
		{
			GetClientArguments(hwnd );
			BackBuffer.AdjustSize(hwnd );
			salemen->Resize(cxClient ,cyClient );
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