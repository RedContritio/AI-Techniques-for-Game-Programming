#ifndef __STANDARD__WINDOW
#define __STANDARD__WINDOW
#include <windows.h>
void InitializeWindowClass(PWNDCLASSEX pWinClass ,WNDPROC WinProc ,
						   HINSTANCE hInstance ,const char* ClassName ) ;
HWND CreateVisibleWindow(const char* ClassName ,const char* TitleName ,
						 int Width ,int Height ,HINSTANCE hInstance ) ;
#endif