#ifndef __BALL__DEF
#define __BALL__DEF
#include <cstdlib>
#include <cstdio>
#include <windows.h>
class BALL
{
	private :
		float px , py ;
		float vx , vy ;
		float __GetRandom(float ,float );
	public :
		BALL(void );
		BALL(float ,float );
		BALL(float ,float ,float ,float );
		void Reset(float ,float );
		void Move(void );
		void Accelate(float ,float );
		friend void Simulate(BALL &ball);
		friend void DrawBall(HDC hdc ,BALL &ball );
};

#endif