#include "StandardWindow.h"

void InitializeWindowClass(PWNDCLASSEX pWinClass ,WNDPROC WinProc ,HINSTANCE hInstance ,const char* ClassName )
{
	pWinClass->cbSize        = sizeof(WNDCLASSEX );
	pWinClass->style         = CS_HREDRAW | CS_VREDRAW ;
	pWinClass->lpfnWndProc   = WinProc ;
	pWinClass->cbClsExtra    = 0 ;
	pWinClass->cbWndExtra    = 0 ;
	pWinClass->hInstance     = hInstance ;
	pWinClass->hIcon         = LoadIcon(NULL ,IDI_APPLICATION );
	pWinClass->hCursor       = LoadCursor(NULL ,IDC_ARROW );
	pWinClass->hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH );
	pWinClass->lpszMenuName  = NULL ;
	pWinClass->lpszClassName = ClassName ;
	pWinClass->hIconSm       = LoadIcon(NULL ,IDI_APPLICATION );
	return ;
}


HWND CreateVisibleWindow(const char* ClassName ,const char* TitleName ,
						 int Width ,int Height ,HINSTANCE hInstance ) 
{
	return CreateWindowEx(WS_EX_CLIENTEDGE ,ClassName ,TitleName ,WS_VISIBLE|WS_OVERLAPPEDWINDOW ,
						  0 ,0 ,Width ,Height ,NULL ,NULL ,hInstance ,NULL );
}