#include "CBody.h"

COLORUN::COLORUN(void )
{
	v = 0 ;
}

COLORUN::COLORUN(const unsigned long &color ):v(color )
{
	;
}

COLORUN& COLORUN::operator = (const unsigned long &color )
{
	v = color ;
	return *this ;
}

CONTRAIL::CONTRAIL(const S2DVector &_p ,int _life ):p(_p ),life(_life ),tlife(_life )
{
	;
}

void CONTRAIL::Update(void )
{
	life -- ;
}

void CONTRAIL::Render(HDC hdc ,COLORUN color )const
{
	COLORUN tmpcolor ;
	double rate = 1.0*life/tlife ;
	tmpcolor.c.r = (BYTE )(rate * color.c.r) ;
	tmpcolor.c.g = (BYTE )(rate * color.c.g) ;
	tmpcolor.c.b = (BYTE )(rate * color.c.b) ;
	SetPixel(hdc ,(int )(p.x ),(int )(p.y ),tmpcolor.v );
}

BODY::BODY(const S2DVector &p ,const S2DVector &v ):pos(p ),velocity(v ),accelator(S2DVector( ))
{
	;
}

BODY::BODY(const S2DVector &p ,const S2DVector &v ,double m ,double r ,COLORUN c ):
		   pos(p ),velocity(v ),accelator(S2DVector( )),mass(m ),radius(r ),color(c )
{
	;
}

BODY::~BODY(void )
{
	;
}

void BODY::SetAccelator(const S2DVector &a )
{
	accelator = a ;
}

bool BODY::OutOfWorld(void )
{
	S2DVector center(cxClient/2 ,cyClient/2 );
	S2DVector dif = pos - center ;
	return (dif.Length() >= 10*center.Length());
}