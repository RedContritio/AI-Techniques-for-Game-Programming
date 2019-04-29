#include "BufferLayer.h"
#include "StandardWindow.h"
#include "LogSpawner.h"

#include "Controller.h"
#include "CTimer.h"

#include "Params.h"

#include <cstdio>
#include <cstdlib>
#include <ctime>

#include "utils.hpp"
#include "defines.h"

using RedContritio::BUFFERLAYER;
using RedContritio::InitializeWindowClass;
using RedContritio::CreateVisibleWindow;
using RedContritio::LogSpawner;

const char *MainClassName = "RedContritio Test MS v0.1 Practice";
const char *MainTitleName = "MineSweeper v0.1";
BUFFERLAYER BackBuffer;
int cxClient, cyClient;
static void GetClientArguments(HWND);

LogSpawner logger;

Controller *pController;

Params params;

LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmdLine, _In_ int nShowCmd)
{
	logger.printf("Application Started\n");

	if ( 0 )
	{
		{
			time_t seed = time(NULL);
			srand((unsigned int) seed);
			logger.printf("seed = %d\n", seed);
		}// Initialize random seed

		if ( AllocConsole() )
		{
		#ifdef __STDC_WANT_SECURE_LIB__
			errno_t errid = freopen_s(NULL, "CONOUT$", "w", stdout);
		#else
			freopen("CONOUT$", "w", stdout);
		#endif
			
		}//Open Console
	}

	WNDCLASSEX winClass;
	InitializeWindowClass(&winClass, WindowProc, hInstance, MainClassName);
	if ( !RegisterClassEx(&winClass) )
	{
		MessageBox(NULL, "Window Registration Failed", "Error", MB_OK);
		return 0;
	}
	HWND hwnd = CreateVisibleWindow(MainClassName, MainTitleName, __WINDOW__WIDTH, __WINDOW__HEIGHT, hInstance);
	if ( !hwnd )
	{
		MessageBox(NULL, "Window Creation Failed", "Error", MB_OK);
		return 0;
	}
	ShowWindow(hwnd, nShowCmd);
	UpdateWindow(hwnd);

	logger.printf("Window Creation Succeeded\n");

	bool EndFlag = false;
	TIMER timer((params.FramesPerSecond != 0) ? (1.0f * params.FramesPerSecond) : __FRAME__PER__SECOND);
	MSG msg;

	timer.Start();
	while ( !EndFlag )
	{
		while ( PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)!= 0 )
		{
			if ( msg.message == WM_QUIT )
			{
				EndFlag = true;
			}
			else
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}


		if ( timer.ReadyForNextFrame() || ( pController && pController->isFastIterating()))
		{
			if ( pController && pController->isSlowIterating() )
			{
				static int t = 5;
				if ( --t > 0 ) continue;
				t = 5;
			}
			if ( pController ) pController->Update();
			InvalidateRect(hwnd, NULL, true);
			UpdateWindow(hwnd);
		}

	}

	UnregisterClass(MainClassName, winClass.hInstance);

	logger.printf("Application Quited\n");
	return 0;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch ( msg )
	{
		case WM_CREATE:
		{
			GetClientArguments(hwnd);
			BackBuffer.Reset(hwnd);
			pController = new Controller;
			break;
		}
		case WM_PAINT:
		{
			PAINTSTRUCT ps;
			BeginPaint(hwnd, &ps);

			BitBlt(BackBuffer.hdc, 0, 0, cxClient, cyClient, NULL, NULL, NULL, WHITENESS);

			if(pController) pController->Render(BackBuffer.hdc);

			BitBlt(ps.hdc, 0, 0, cxClient, cyClient, BackBuffer.hdc, NULL, NULL, SRCCOPY);

			EndPaint(hwnd, &ps);
			break;
		}
		case WM_KEYUP:
		{

			switch ( wParam )
			{
				case 'S':
				{
					if ( pController ) pController->ToggleSlowIterating();
					break;
				}
				case 'F':
				{
					if ( pController ) pController->ToggleFastIterating();
					break;
				}
				case 'V':
				{
					if ( pController ) pController->Watch();
					break;
				}
				case VK_RETURN:
				{
					break;
				}
				case VK_SPACE:
				{
					if ( pController )pController->Pause();
					break;
				}
				case VK_LEFT: case VK_UP:
				case VK_RIGHT: case VK_DOWN:
				{
					break;
				}
				case VK_ESCAPE:
				{
					PostQuitMessage(0);
					break;
				}
			}
			break;
		}
		case WM_SIZE:
		{
			GetClientArguments(hwnd);
			BackBuffer.Adjust(hwnd);
			break;
		}
		case WM_COMMAND:
		{
			break;
		}
		case WM_DESTROY:
		{
			if ( pController )
			{
				delete pController;
				pController = NULL;
			}
			BackBuffer.Delete();
			PostQuitMessage(0);
			break;
		}
	}
	return DefWindowProc(hwnd, msg, wParam, lParam);
}


static inline void GetClientArguments(HWND hwnd)
{
	RECT rect;
	GetClientRect(hwnd, &rect);
	params.WindowWidth = cxClient = rect.right;
	params.WindowHeight = cyClient = rect.bottom;
}