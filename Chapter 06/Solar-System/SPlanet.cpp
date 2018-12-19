#include "CSolar.h"
CONTRAIL::CONTRAIL(double x ,double y )
{
	pos.x = x ;
	pos.y = y ;
	life = CONTRAIL__DEFAULT__MAX__LIFE ;
}

CONTRAIL::CONTRAIL(S2DVector _pos)
{
	pos = _pos ;
	life = CONTRAIL__DEFAULT__MAX__LIFE ;
}

PLANET::PLANET(void ):mass(PLANET__DEFAULT__MASS ),radius(PLANET__DEFAULT__RADIUS ),color(PLANET__DEFAULT__COLOR )
{
	TimeToSpawnContrail = CONTRAIL__DEFAULT__INTERVAL ;
}

PLANET::~PLANET(void )
{
	;
}

void PLANET::Update(double TimeElapsed ,const GravityVariables& Gparas )
{
	S2DVector a ;
	double R = CalculateDistance(pos ,Gparas._pos );
	double angle = CalculateAngle(pos ,Gparas._pos );
	double tmp_a = -G*(Gparas._mass / (R*R ));
	a.x = tmp_a * cos(angle );
	a.y = tmp_a * sin(angle );
	a *= TimeElapsed ;
	velocity += (a * TimeElapsed * dtScale ) ;
	pos += velocity ;
	return ;
}

void PlanetRender(HDC hdc , const PLANET& planet )
{
	HBRUSH hBrush = CreateSolidBrush(planet.color);
	HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, hBrush);

	HPEN hPen = CreatePen(PS_SOLID ,1 ,planet.color );
	HPEN hOldPen = (HPEN)SelectObject(hdc ,hPen );

	Ellipse(hdc ,(int)(planet.pos.x -planet.radius ),(int)(planet.pos.y -planet.radius ),
				 (int)(planet.pos.x +planet.radius ),(int)(planet.pos.y +planet.radius ));

	SelectObject(hdc ,hOldBrush );
	SelectObject(hdc ,hOldPen );

	DeleteObject(hBrush );
	DeleteObject(hPen );

	return ;
}



double CalculateAngle(const S2DVector &p1 ,const S2DVector &p2 )
{
	double dif_x = p1.x - p2.x ;
	double dif_y = p1.y - p2.y ;
	double angle ;
	if(dif_x == 0 )
	{
		if(dif_y >= 0 )
		{
			angle = PI/2 ;
		}
		else
		{
			angle = 1.5*PI ;
		}
	}
	else
	{
		angle = atan(abs(dif_y ) / abs(dif_x) );
		if(dif_y >= 0 )
		{
			if(dif_x < 0 )
			{
				angle = PI - angle ;
			}
			else
			{
				angle = angle ;
			}
		}
		else
		{
			if(dif_x < 0 )
			{
				angle = PI + angle ;
			}
			else
			{
				angle = -angle ;
			}
		}
	}
	return angle ;
}