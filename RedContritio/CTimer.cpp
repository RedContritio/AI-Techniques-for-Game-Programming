#include "CTimer.h"
namespace RedContritio
{
	Timer::Timer(void ):currentTime(0ull ),lastTime(0ull ),nextTime(0ull ),frameTime(0ull ),
						  timeElapsed(0 ),fps(0 )
	{
		QueryPerformanceFrequency((LARGE_INTEGER*)(&perFrameCountFreq));
		timeScale = 1.0 / perFrameCountFreq ;  
	}
	
	Timer::Timer(double FPS ):currentTime(0ull ),lastTime(0ull ),nextTime(0ull ),frameTime(0ull ),
						  		timeElapsed(0 ),fps(FPS )
	{
		QueryPerformanceFrequency((LARGE_INTEGER*)(&perFrameCountFreq));
		timeScale = 1.0 / perFrameCountFreq ;
		frameTime = (ULONGLONG )(perFrameCountFreq / fps );
	}
	
	void Timer::Start(void )
	{
		QueryPerformanceCounter((LARGE_INTEGER*)(&lastTime ));
		currentTime = lastTime ;
		nextTime = lastTime + frameTime ;
		return ;
	}
	
	bool Timer::ReadyForNextFrame(void )
	{
		if(!fps )return false ;
		QueryPerformanceCounter((LARGE_INTEGER*)(&currentTime ));
		if(currentTime >nextTime )
		{
			timeElapsed = (currentTime - lastTime ) * timeScale ;
			lastTime = currentTime ;
			nextTime = currentTime + frameTime ;
			return true ;
		}
		return false ;
	}
	
	double Timer::GetTimeElapsed(void )
	{
		return timeElapsed ;
	}
	
	double Timer::TimeElapsed(void )
	{
		QueryPerformanceCounter((LARGE_INTEGER*)(&currentTime ));
		timeElapsed = (currentTime - lastTime ) * timeScale ;
		lastTime = currentTime ;
		return timeElapsed ;
	}
}
