#include "Balls.h"
float BALL::__GetRandom(float low ,float high )
{
	return low+(high-low)*(1.0*rand()/RAND_MAX);
}

void BALL::Reset(float X ,float Y )
{
	px = __GetRandom(0 ,X );
	py = __GetRandom(0 ,Y );
	vx = __GetRandom(-0.25 ,0.25 );
	vy = __GetRandom(-0.25 ,0.25 );
}

BALL::BALL(void )
{
	;
}

BALL::BALL(float px ,float py ):px(px ),py(py )
{
	;
}

BALL::BALL(float px ,float py ,float vx ,float vy ):px(px ),py(py ),vx(vx ),vy(vy )
{
	;
}

void BALL::Move(void )
{
	px += vx ;
	py += vy ;
	return ;
}

void BALL::Accelate(float ax ,float ay )
{
	vx += ax ;
	vy += ay ;
}