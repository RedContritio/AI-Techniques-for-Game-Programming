#ifndef STDWINDOW__RC__INC
#define STDWINDOW__RC__INC
#include <windows.h>
namespace RedContritio
{
	void InitializeWindowClass(PWNDCLASSEX pWinClass ,WNDPROC WinProc ,
	                           HINSTANCE hInstance ,const char* ClassName ) ;
	HWND CreateVisibleWindow(const char* ClassName ,const char* TitleName ,
	                         int Width ,int Height ,HINSTANCE hInstance ) ;
}
#endif
