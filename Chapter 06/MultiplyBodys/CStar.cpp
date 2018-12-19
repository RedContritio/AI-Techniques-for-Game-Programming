#include "CStar.h"

extern int cxClient ,cyClient ;
STAR::STAR(void ):BODY(S2DVector(cxClient/2 ,cyClient/2 ),S2DVector( ),STAR__DEFAULT__MASS ,
					   STAR__DEFAULT__RADIUS ,COLORUN(STAR__DEFAULT__COLOR ))
{
	;
}

STAR::STAR(double x ,double y ):BODY(S2DVector(x ,y ),S2DVector( ),STAR__DEFAULT__MASS ,
									 STAR__DEFAULT__RADIUS ,COLORUN(STAR__DEFAULT__COLOR ))
{
	;
}

STAR::STAR(const S2DVector &_pos ):BODY(_pos ,S2DVector( ),STAR__DEFAULT__MASS ,
										STAR__DEFAULT__RADIUS ,COLORUN(STAR__DEFAULT__COLOR ))
{
	;
}

STAR::STAR(const S2DVector &_pos ,const S2DVector &_v ):BODY(_pos ,S2DVector(_v ),STAR__DEFAULT__MASS ,
		   STAR__DEFAULT__RADIUS ,COLORUN(STAR__DEFAULT__COLOR ))
{
	;
}

STAR::~STAR(void )
{
	;
}

void STAR::Update(double TimeElapsed )
{
	for(size_t i=0 ;i<contrails.size() ;i++ )
	{
		contrails[i].life -- ;
		if(contrails[i].life <= 0 )
			contrails.erase(contrails.begin()+i );
	}
	velocity += accelator ;//* TimeElapsed ;
	pos += velocity ;//*TimeElapsed ;
	contrails.push_back(CONTRAIL(pos ,3*CONTRAIL__DEFAULT__MAX__LIFE ));
}

void STAR::Render(HDC hdc )
{
	for(size_t i=0 ;i<contrails.size() ;i++ )
	{
		contrails[i].Render(hdc ,color );
	}
	HBRUSH hBrush = CreateSolidBrush(color.v );
	HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, hBrush);

	HPEN hPen = CreatePen(PS_SOLID ,1 ,color.v );
	HPEN hOldPen = (HPEN)SelectObject(hdc ,hPen );

	Ellipse(hdc ,(int)(pos.x - radius ),(int)(pos.y - radius ),
		(int)(pos.x + radius ),(int)(pos.y + radius ));

	SelectObject(hdc ,hOldBrush );
	SelectObject(hdc ,hOldPen );

	DeleteObject(hBrush );
	DeleteObject(hPen );
}

void STAR::RenderVelocity(HDC hdc )
{
	HPEN hPen = CreatePen(PS_SOLID ,1 ,RGB(~(color.c.r ),~(color.c.g ),~(color.c.b )) );
	HPEN hOldPen = (HPEN)SelectObject(hdc ,hPen );

	S2DVector ControlLine = velocity ;
	ControlLine /= dvScale ;

	MoveToEx(hdc ,(int)(pos.x ),(int)(pos.y ),NULL );
	LineTo(hdc ,(int)((pos+ControlLine).x ),(int)((pos+ControlLine).y ));

	SelectObject(hdc ,hOldPen );

	DeleteObject(hPen );
}

void SpecialStarRender(HDC hdc ,const S2DVector &p1 ,const S2DVector &p2 )
{
	HBRUSH hBrush = CreateSolidBrush(STAR__DEFAULT__COLOR );
	HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, hBrush);

	HPEN hPen = CreatePen(PS_SOLID ,1 ,STAR__DEFAULT__COLOR );
	HPEN hOldPen = (HPEN)SelectObject(hdc ,hPen );

	Ellipse(hdc ,(int)(p1.x - STAR__DEFAULT__RADIUS ),(int)(p1.y - STAR__DEFAULT__RADIUS ),
		(int)(p1.x + STAR__DEFAULT__RADIUS ),(int)(p1.y + STAR__DEFAULT__RADIUS ));

	MoveToEx(hdc ,(int)(p1.x ),(int)(p1.y ),NULL );
	LineTo(hdc ,(int)(p2.x ),(int)(p2.y ));

	SelectObject(hdc ,hOldBrush );
	SelectObject(hdc ,hOldPen );

	DeleteObject(hBrush );
	DeleteObject(hPen );
}

