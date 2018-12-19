#include <windows.h>
#include "StandardWindow.h"
const char* MainClassName = "Simple but boring Program" ;
const char* MainTitleName = "Boring Program" ;
LRESULT CALLBACK WindowProc(HWND hwnd ,
							UINT msg ,
							WPARAM wParam ,
							LPARAM lParam );

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
	while(GetMessage(&msg ,NULL ,0 ,0 ))
	{
		TranslateMessage(&msg ) ;
		DispatchMessage(&msg ) ;
	}
	return 0 ;
}


LRESULT CALLBACK WindowProc(HWND hwnd ,
							UINT msg ,
							WPARAM wParam ,
							LPARAM lParam )
{
	switch(msg )
	{
		case WM_CREATE :
		{
			return 0 ;
		}
		case WM_DESTROY :
		{
			PostQuitMessage(0 );
			return 0 ;
		}
		default :
			break ;
	}
	return DefWindowProc(hwnd ,msg ,wParam ,lParam );
}