#include <windows.h>
#include "defines.h"
#include "BufferLayer.h"
#include "StandardWindow.h"
#include "LogSpawner.h"
#include "CTimer.h"
#include <cstdio>
#include <cstdlib>
#include <ctime>

#include "map.h"
#include "population.h"

#include "resource.h"

using RedContritio::BUFFERLAYER ;
using RedContritio::InitializeWindowClass ;
using RedContritio::CreateVisibleWindow ;
using RedContritio::LogSpawner ;

const char* MainClassName = "Improved Traveling Saleman Problem" ;
const char* MainTitleName = "Improved TSP" ;
BUFFERLAYER BackBuffer ;
int cxClient ,cyClient ;
static void GetClientArguments(HWND );

HMENU hMenu ;

LogSpawner loger ;

POPULATION *pPopulation ;

LRESULT CALLBACK WindowProc(HWND ,UINT ,WPARAM ,LPARAM );

int WINAPI WinMain(HINSTANCE hInstance ,HINSTANCE hPrevInstance ,
				   LPSTR lpCmdLine ,int nShowCmd )
{
	loger.lprint("Application Started\n" );
	{
		time_t t = time(NULL );
		srand(( unsigned)t );
		loger.lprint("seed = %d\n",t );
	}
	/*
	if(AllocConsole())
	{
		freopen("CONOUT$","w",stdout);
	}//Open Console
	*/

	WNDCLASSEX winClass ;
	InitializeWindowClass(&winClass ,WindowProc ,hInstance ,MainClassName );
	winClass.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1 );
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
	CTimer timer(80 );
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
			if(pPopulation->Started() )
			{
				pPopulation->Epoch( );
			}
			InvalidateRect(hwnd ,NULL ,true );
			UpdateWindow(hwnd );
		}

	}

	UnregisterClass(MainClassName ,winClass.hInstance );
	
	if(pPopulation )
	{
		delete pPopulation ;
	}

	loger.lprint("Application Quited\n" );
	return 0 ;
}

LRESULT CALLBACK WindowProc(HWND hwnd ,UINT msg ,WPARAM wParam ,LPARAM lParam )
{
	hMenu = GetMenu(hwnd );
	switch(msg )
	{
		case WM_CREATE :
		{
			srand(( unsigned)time(0 ));
			GetClientArguments(hwnd );
			BackBuffer.ResetBufferLayer(hwnd );

			pPopulation = new POPULATION(__MUTATION__RATE ,__CROSSOVER__RATE ,__POPULATION__SIZE ,
										 __CITIES__NUM ,cxClient ,cyClient );

			pPopulation->SelectMutationType(MutationType_Exchange );
			pPopulation->SelectCrossoverType(CrossoverType_PartiallyMapping );
			
			CheckMenuRadioItem(hMenu ,MutationType_BEGIN ,MutationType_END ,ID_MENU_MUTATION01 ,MF_BYCOMMAND );
			CheckMenuRadioItem(hMenu ,CrossoverType_BEGIN ,CrossoverType_END ,ID_MENU_XOVER01 ,MF_BYCOMMAND );
			break ;
		}
		case WM_PAINT :
		{
			PAINTSTRUCT ps ;
			BeginPaint(hwnd ,&ps );

			BitBlt(BackBuffer.GetHdc( ),0 ,0 ,cxClient ,cyClient ,NULL ,NULL ,NULL ,WHITENESS );
			if(pPopulation )
			{
				pPopulation->Render(BackBuffer.GetHdc() ,cxClient ,cyClient );
			}
			BitBlt(ps.hdc ,0 ,0 ,cxClient ,cyClient ,BackBuffer.GetHdc( ),NULL ,NULL ,SRCCOPY );

			EndPaint(hwnd ,&ps );
			break ;
		}
		case WM_KEYUP :
		{
			switch(wParam )
			{
				case VK_RETURN :
				{
					pPopulation->Run(hwnd );
					break ;
				}
				case VK_SPACE :
				{
					pPopulation->Stop( );
					break ;
				}

			}
			break ;
		}
		case WM_SIZE :
		{
			GetClientArguments(hwnd );
			BackBuffer.AdjustSize(hwnd );
			pPopulation->Resize(cxClient ,cyClient );
			break ;
		}
		case WM_COMMAND :
		{
			switch(wParam )
			{
				case ID_MENU_MUTATION01 : // Exchange Mutation
				{
					pPopulation->SelectMutationType(MutationType_Exchange );
					InvalidateRect(hwnd, NULL, TRUE);
					UpdateWindow(hwnd);
					CheckMenuRadioItem(hMenu ,MutationType_BEGIN ,MutationType_END ,ID_MENU_MUTATION01 ,MF_BYCOMMAND );
					break ;
				}
				case ID_MENU_MUTATION02 : // test Mutation
				{
					pPopulation->SelectMutationType(MutationType_Displacement );
					InvalidateRect(hwnd, NULL, TRUE);
					UpdateWindow(hwnd);
					CheckMenuRadioItem(hMenu ,MutationType_BEGIN ,MutationType_END ,ID_MENU_MUTATION02 ,MF_BYCOMMAND );
					break ;
				}
				case ID_MENU_MUTATION03 : // test Mutation
				{
					pPopulation->SelectMutationType(MutationType_Scramble );
					InvalidateRect(hwnd, NULL, TRUE);
					UpdateWindow(hwnd);
					CheckMenuRadioItem(hMenu ,MutationType_BEGIN ,MutationType_END ,ID_MENU_MUTATION03 ,MF_BYCOMMAND );
					break ;
				}
				case ID_MENU_MUTATION04 : // test Mutation
				{
					pPopulation->SelectMutationType(MutationType_Insert );
					InvalidateRect(hwnd, NULL, TRUE);
					UpdateWindow(hwnd);
					CheckMenuRadioItem(hMenu ,MutationType_BEGIN ,MutationType_END ,ID_MENU_MUTATION04 ,MF_BYCOMMAND );
					break ;
				}
				case ID_MENU_MUTATION05 : // test Mutation
				{
					pPopulation->SelectMutationType(MutationType_Automatic );
					InvalidateRect(hwnd, NULL, TRUE);
					UpdateWindow(hwnd);
					CheckMenuRadioItem(hMenu ,MutationType_BEGIN ,MutationType_END ,ID_MENU_MUTATION05 ,MF_BYCOMMAND );
					break ;
				}
				case ID_MENU_XOVER01 :
				{
					pPopulation->SelectCrossoverType(CrossoverType_PartiallyMapping );
					InvalidateRect(hwnd, NULL, TRUE);
					UpdateWindow(hwnd);
					CheckMenuRadioItem(hMenu ,CrossoverType_BEGIN ,CrossoverType_END ,ID_MENU_XOVER01 ,MF_BYCOMMAND );
					break ;
				}
				case ID_MENU_XOVER02 :
				{
					pPopulation->SelectCrossoverType(CrossoverType_OrderBasedOperator );
					InvalidateRect(hwnd, NULL, TRUE);
					UpdateWindow(hwnd);
					CheckMenuRadioItem(hMenu ,CrossoverType_BEGIN ,CrossoverType_END ,ID_MENU_XOVER02 ,MF_BYCOMMAND );
					break ;
				}
				case ID_MENU_XOVER03 :
				{
					pPopulation->SelectCrossoverType(CrossoverType_PositionBasedOperator );
					InvalidateRect(hwnd, NULL, TRUE);
					UpdateWindow(hwnd);
					CheckMenuRadioItem(hMenu ,CrossoverType_BEGIN ,CrossoverType_END ,ID_MENU_XOVER03 ,MF_BYCOMMAND );
					break ;
				}
			}
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