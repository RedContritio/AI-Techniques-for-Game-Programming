#include "Balls.h"
float BALL::__GetRandom(float low ,float high )
{
	return (float)(low+(high-low)*(1.0*rand()/RAND_MAX));
}

void BALL::Reset(float X ,float Y )
{
	m_px = __GetRandom(0 ,X );
	m_py = __GetRandom(0 ,Y );
	m_vx = __GetRandom(-2 ,2 );
	m_vy = __GetRandom(-2 ,2 );
}

BALL::BALL(void )
{
	;
}

BALL::BALL(float px ,float py ):m_px(px ),m_py(py )
{
	;
}

BALL::BALL(float px ,float py ,float vx ,float vy ):m_px(px ),m_py(py ),m_vx(vx ),m_vy(vy )
{
	;
}

void BALL::Move(void )
{
	m_px += m_vx ;
	m_py += m_vy ;
	return ;
}

void BALL::Accelate(float ax ,float ay )
{
	m_vx += ax ;
	m_vy += ay ;
}