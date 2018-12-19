#include <vector>
#include <cstdlib>
#include <ctime>
#include "BufferLayer.h"
#include "StandardWindow.h"
#include "Balls.h"
#include "resource.h"

using RedContritio::BUFFERLAYER ;
using RedContritio::InitializeWindowClass ;
using RedContritio::CreateVisibleWindow ;

using std::vector ;

const char* MainClassName = "DialogTestProject" ;
const char* MainTitleName = "Dialog & BackBuffer" ;

int cxClient ,cyClient ;
BUFFERLAYER BackBuffer ;

vector<BALL> Balls ;
int NumOfBalls = 10 ;
int RadiusOfBalls = 10 ;
void Simulate(BALL &ball);
void DrawBall(HDC hdc ,BALL &ball );

LRESULT CALLBACK WindowProc(HWND hwnd ,UINT msg ,
							WPARAM wParam ,LPARAM lParam );
BOOL CALLBACK DialogProc(HWND hwnd ,UINT msg ,
						 WPARAM wParam ,LPARAM lParam );

int WINAPI WinMain(HINSTANCE hInstance ,HINSTANCE hPrevInstance ,LPSTR lpCmdLine ,int nShowCmd )
{
	srand(( unsigned)time(0 ));
	WNDCLASSEX wc ;
	InitializeWindowClass(&wc ,WindowProc ,hInstance ,MainClassName );
	if(!RegisterClassEx(&wc ))
	{
		MessageBox(NULL ,"Window Registration Failed" ,"Error" ,MB_OK );
		return 0 ;
	}
	HWND hwnd = CreateVisibleWindow(MainClassName ,MainTitleName ,640 ,480 ,hInstance );

	MSG msg ;
	bool EndFlag = false ;

	while(!EndFlag )
	{
		while(PeekMessage(&msg ,NULL ,0 ,0 ,PM_REMOVE ))
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
		
		
		while(Balls.size() > NumOfBalls )Balls.pop_back();
		while(Balls.size() < NumOfBalls )
		{
			BALL temp ;
			temp.Reset(cxClient ,cyClient );
			Balls.push_back(temp );
		}
		for(std::vector<BALL>::iterator itor = Balls.begin() ;itor != Balls.end() ;itor ++ )
		{
			Simulate(*itor );
		}

		InvalidateRect(hwnd ,NULL ,true );
		UpdateWindow(hwnd );
	}
	return msg.wParam ;
}

LRESULT CALLBACK WindowProc(HWND hwnd ,UINT msg ,
							WPARAM wParam ,LPARAM lParam )
{
	static HINSTANCE hInstance ;
	switch(msg )
	{
		case WM_CREATE :
		{
			RECT rect ;
			GetWindowRect(hwnd ,&rect );
			cxClient = rect.right ;
			cyClient = rect.bottom ;

			hInstance = ((LPCREATESTRUCT)lParam)->hInstance ;

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
			BitBlt(ps.hdc ,0 ,0 ,cxClient ,cyClient ,BackBuffer.GetHdc() ,NULL ,NULL ,SRCCOPY );
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
		case WM_RBUTTONUP :
		{
			DialogBox(hInstance ,MAKEINTRESOURCE(IDD_DIALOG1) ,hwnd ,DialogProc );
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

BOOL CALLBACK DialogProc(HWND hwnd ,UINT msg ,
						 WPARAM wParam ,LPARAM lParam )
{
	HWND hwndNum = GetDlgItem(hwnd ,IDC_EDIT1 );
	HWND hwndRange = GetDlgItem(hwnd ,IDC_EDIT2 );
	switch(msg )
	{
		case WM_INITDIALOG :
		{
			char NumStr[10] = "" ,RStr[10] = "" ;
			sprintf(NumStr ,"%d" ,NumOfBalls );
			sprintf(RStr ,"%d" ,RadiusOfBalls );
			SetWindowText(hwndNum ,NumStr );
			SetWindowText(hwndRange ,RStr );
			return true ;
			break ;
		}
		case WM_COMMAND :
		{
			switch(LOWORD(wParam ))
			{
				case IDOK :
				{
					char NumStr[10] = "" ,RStr[10] = "";
					GetWindowText(hwndNum ,NumStr ,4 );
					GetWindowText(hwndRange ,RStr ,4 );
					NumOfBalls = atoi(NumStr );
					RadiusOfBalls = atoi(RStr );
					EndDialog(hwnd ,0 );
					return true ;
				}
				case IDCANCEL :
				{
					EndDialog(hwnd ,0 );
					return true ;
				}
				default :
					break ;
			}
			break ;
		}
		default :
			break ;
	}
	return false ;
}

void Simulate(BALL &ball)
{
	if( ball.px+RadiusOfBalls > cxClient && ball.vx > 0 )ball.vx = -ball.vx ;
	if( ball.px-RadiusOfBalls < 0 && ball.vx < 0 )ball.vx = -ball.vx ;
	if( ball.py+RadiusOfBalls > cyClient && ball.vy > 0 )ball.vy = -ball.vy ;
	if( ball.py+RadiusOfBalls < cyClient )ball.Accelate(0 ,0.005 );
	ball.Move() ;
}

void DrawBall(HDC hdc ,BALL &ball )
{
	Ellipse(hdc ,ball.px - RadiusOfBalls ,ball.py - RadiusOfBalls ,
				 ball.px + RadiusOfBalls ,ball.py + RadiusOfBalls );
}