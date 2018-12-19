#include <vector>
#include "BufferLayer.h"
#include "StandardWindow.h"
#include "Balls.h"
#include "CTimer.h"

using RedContritio::BUFFERLAYER ;
using RedContritio::InitializeWindowClass ;
using RedContritio::CreateVisibleWindow ;

using std::vector ;

const int FRAMES_PER_SECOND = 120 ;

const char* MainClassName = "CTimerTestProject" ;
const char* MainTitleName = "CTimer" ;

int cxClient ,cyClient ;
BUFFERLAYER BackBuffer ;

vector<BALL> Balls ;
int NumOfBalls = 40 ;
int RadiusOfBalls = 30 ;

void Simulate(BALL &ball);
void DrawBall(HDC hdc ,BALL &ball );

LRESULT CALLBACK WindowProc(HWND hwnd ,UINT msg ,
							WPARAM wParam ,LPARAM lParam );

int WINAPI WinMain(HINSTANCE hInstance ,HINSTANCE hPrevInstance ,
				   LPSTR lpCmdLine ,int nShowCmd )
{
	WNDCLASSEX wc ;
	InitializeWindowClass(&wc ,WindowProc ,hInstance ,MainClassName );
	if(!RegisterClassEx(&wc ))
	{
		MessageBox(NULL ,"Window Registration Failed" ,"Error" ,MB_OK );
		return 0 ;
	}
	HWND hwnd = CreateVisibleWindow(MainClassName ,MainTitleName ,640 ,480 ,hInstance );
	if(!hwnd )
	{
		MessageBox(NULL ,"Window Creation Failed" ,"Error" ,0 );
		return 0 ;
	}
	
	ShowWindow(hwnd ,nShowCmd );
	UpdateWindow(hwnd );

	bool EndFlag = false ;
	CTimer timer(FRAMES_PER_SECOND );
	MSG msg ;
	
	timer.Start( );
	while(!EndFlag )
	{
		while(PeekMessage(&msg ,NULL ,0 ,0 ,PM_REMOVE ) )
		{
			if(msg.message ==WM_QUIT )
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
			while(Balls.size() > NumOfBalls )Balls.pop_back();
			while(Balls.size() < NumOfBalls )
			{
				BALL NewBall;
				NewBall.Reset(cxClient ,cyClient );
				Balls.push_back(NewBall );
			}
			for(std::vector<BALL>::iterator itor = Balls.begin() ;itor != Balls.end() ;itor ++ )
			{
				Simulate(*itor );
			}

			InvalidateRect(hwnd ,NULL ,true );
			UpdateWindow(hwnd );
		}
	}

	UnregisterClass(MainClassName ,wc.hInstance );
	return 0 ;
}

LRESULT CALLBACK WindowProc(HWND hwnd ,UINT msg ,
							WPARAM wParam ,LPARAM lParam )
{
	switch(msg )
	{
		case WM_CREATE :
		{
			RECT rect ;
			GetWindowRect(hwnd ,&rect );
			cxClient = rect.right ;
			cyClient = rect.bottom ;

			BackBuffer.ResetBufferLayer(hwnd );
			break ;
		}
		case WM_PAINT :
		{
			PAINTSTRUCT ps ;
			BeginPaint(hwnd ,&ps );
			BitBlt(BackBuffer.GetHdc() ,0 ,0 ,cxClient ,cyClient ,NULL ,NULL ,NULL ,WHITENESS );
			HBRUSH brush = (HBRUSH)GetStockObject(NULL_BRUSH);
			SelectObject(BackBuffer.GetHdc() ,brush );
			for(std::vector<BALL>::iterator itor = Balls.begin() ;itor != Balls.end() ;itor ++ )
			{
				DrawBall(BackBuffer.GetHdc() ,*itor );
			}
			BitBlt(ps.hdc ,0 ,0 ,cxClient ,cyClient ,BackBuffer.GetHdc() ,0 ,0 ,SRCCOPY );
			EndPaint(hwnd ,&ps );
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
			BackBuffer.DeleteBufferLayer() ;
			PostQuitMessage(0 );
			return 0 ;
		}
		default :
			break ;
	}
	return DefWindowProc(hwnd ,msg ,wParam ,lParam );
}


void Simulate(BALL &ball)
{
	if(ball.px+RadiusOfBalls>cxClient&&ball.vx>0)ball.vx=-ball.vx;
	if(ball.px-RadiusOfBalls<0&&ball.vx<0)ball.vx=-ball.vx;
	if(ball.py+RadiusOfBalls>cyClient&&ball.vy>0)ball.vy=-ball.vy;
	if(ball.py+RadiusOfBalls<cyClient)ball.Accelate((float)0 ,(float)0.1 );
	ball.Move( );
	return ;
}

void DrawBall(HDC hdc ,BALL &ball )
{
	Ellipse(hdc ,ball.px-RadiusOfBalls ,ball.py-RadiusOfBalls ,
				 ball.px+RadiusOfBalls ,ball.py+RadiusOfBalls );
	return ;
}