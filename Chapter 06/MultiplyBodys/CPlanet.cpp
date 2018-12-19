#include "CPlanet.h"
PLANET::PLANET(void ):BODY(S2DVector( ),S2DVector( ),PLANET__DEFAULT__MASS ,
					  PLANET__DEFAULT__RADIUS ,COLORUN(PLANET__DEFAULT__COLOR ))
{
	;
}

PLANET::PLANET(double x ,double y ):BODY(S2DVector(x ,y ),S2DVector( ),PLANET__DEFAULT__MASS ,
									PLANET__DEFAULT__RADIUS ,COLORUN(PLANET__DEFAULT__COLOR ))
{
	;
}

PLANET::PLANET(const S2DVector &_pos ):BODY(_pos ,S2DVector( ),PLANET__DEFAULT__MASS ,
									   PLANET__DEFAULT__RADIUS ,COLORUN(PLANET__DEFAULT__COLOR ))
{
	;
}

PLANET::PLANET(const S2DVector &_pos ,const S2DVector &_v ):
			   BODY(_pos ,_v ,PLANET__DEFAULT__MASS ,PLANET__DEFAULT__RADIUS ,COLORUN(PLANET__DEFAULT__COLOR ))
{
	;
}

PLANET::~PLANET(void )
{
	;
}

void PLANET::Update(double )
{
	for(size_t i=0 ;i<contrails.size() ;i++ )
	{
		contrails[i].life -- ;
		if(contrails[i].life <= 0 )
			contrails.erase(contrails.begin()+i );
	}
	velocity += accelator ;//* TimeElapsed ;
	pos += velocity ;//*TimeElapsed ;
	contrails.push_back(CONTRAIL(pos ,CONTRAIL__DEFAULT__MAX__LIFE ));
}

void PLANET::Render(HDC hdc )
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

void PLANET::RenderVelocity(HDC hdc )
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

void SpecialPlanetRender(HDC hdc ,const S2DVector &p1 ,const S2DVector &p2 )
{
	HBRUSH hBrush = CreateSolidBrush(PLANET__DEFAULT__COLOR );
	HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, hBrush);

	HPEN hPen = CreatePen(PS_SOLID ,1 ,PLANET__DEFAULT__COLOR );
	HPEN hOldPen = (HPEN)SelectObject(hdc ,hPen );

	Ellipse(hdc ,(int)(p1.x - PLANET__DEFAULT__RADIUS ),(int)(p1.y - PLANET__DEFAULT__RADIUS ),
		(int)(p1.x + PLANET__DEFAULT__RADIUS ),(int)(p1.y + PLANET__DEFAULT__RADIUS ));

	MoveToEx(hdc ,(int)(p1.x ),(int)(p1.y ),NULL );
	LineTo(hdc ,(int)(p2.x ),(int)(p2.y ));

	SelectObject(hdc ,hOldBrush );
	SelectObject(hdc ,hOldPen );

	DeleteObject(hBrush );
	DeleteObject(hPen );
}