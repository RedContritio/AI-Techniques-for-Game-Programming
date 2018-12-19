#include "CSolar.h"
ORBIT::ORBIT(SOLAR* source ,double _radius ):CenterObject(source ),radius(_radius )
{
	;
}

ORBIT::~ORBIT(void )
{
	;
}

void ORBIT::Update(double TimeElapsed )
{
	for(size_t i=0 ;i<Surroundings.size() ;i++ )
	{
		angles[i] += omega ;
		if(angles[i] > 2*PI )angles[i] -= 2*PI ;
		if(angles[i] < -PI )angles[i] += 2*PI ;
		Surroundings[i].pos.x = CenterObject->pos.x + radius * cos(angles[i] );
		Surroundings[i].pos.y = CenterObject->pos.y + radius * sin(angles[i] );
	}
}

void OrbitRender(HDC hdc ,const ORBIT& orbit )
{
	HBRUSH hBrush = (HBRUSH )GetStockObject(NULL_BRUSH );
	HBRUSH hOldBrush = (HBRUSH )SelectObject(hdc ,hBrush );

	HPEN hPen = CreatePen(PS_SOLID ,1 ,ORBIT__DEFAULT__COLOR );
	HPEN hOldPen = (HPEN)SelectObject(hdc ,hPen );

	double x = orbit.CenterObject->pos.x ;
	double y = orbit.CenterObject->pos.y ;
	double r = orbit.radius ;
	Ellipse(hdc ,(int)(x -r ),(int)(y -r ),
		(int)(x +r ),(int)(y +r ));

	SelectObject(hdc ,hOldBrush );
	SelectObject(hdc ,hOldPen );

	DeleteObject(hBrush );
	DeleteObject(hPen );

	for(size_t i=0 ;i < orbit.Surroundings.size() ;i++ )
	{
		PlanetRender(hdc ,orbit.Surroundings[i] );
	}

	return ;
}

double ORBIT::addPlanet(PLANET &planet )
{
	Surroundings.push_back(planet );

	double dif_x = planet.pos.x - CenterObject->pos.x ;
	double dif_y = planet.pos.y - CenterObject->pos.y ;

	double angle = CalculateAngle(planet.pos ,CenterObject->pos );
	angles.push_back(angle );
	return angle ;
}

void ORBIT::DeletePlanet(size_t id )
{
	if(id >= Surroundings.size() )return ;
	Surroundings.erase(Surroundings.begin()+id );
	angles.erase(angles.begin()+id );
	return ;
}