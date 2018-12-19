#include "CSolar.h"
static double PLANET_SPAWN__X ,PLANET_SPAWN__Y ;
static bool PLANET_SPAWN__READY ;
static bool AwayFromScreen(S2DVector pos );


SOLAR::SOLAR(void ):pos(S2DVector(cxClient/2 ,cyClient/2 )),mass(SOLAR__DEFAULT__MASS ),
radius(SOLAR__DEFAULT__RADIUS ),color(SOLAR__DEFAULT__COLOR )
{
	;
}

SOLAR::SOLAR(S2DVector _pos ,double _mass = SOLAR__DEFAULT__MASS ,double _radius = SOLAR__DEFAULT__RADIUS ,
			 unsigned int _color = SOLAR__DEFAULT__COLOR ):pos(pos ),mass(_mass ),radius(_radius ),color(_color )
{
	;
}

SOLAR::~SOLAR(void )
{
	;
}

void SOLAR::Render(HDC hdc )
{
	for(size_t i=0 ;i<orbits.size() ;i++ )
	{
		OrbitRender(hdc ,orbits[i] );
	}
	for(size_t i=0 ;i<contrails.size() ;i++ )
	{
		ContrailsRender(hdc ,contrails[i] );
	}
	for(size_t i=0 ;i<bombs.size() ;i++ )
	{
		BombRender(hdc ,bombs[i] );
	}
	for(size_t i=0 ;i<planets.size() ;i++ )
	{
		PlanetRender(hdc ,planets[i] );
	}
	if(PLANET_SPAWN__READY )
	{
		SpecialRender(hdc );;
	}

	HBRUSH hBrush = CreateSolidBrush(color );
	HBRUSH hOldBrush = (HBRUSH )SelectObject(hdc ,hBrush );
	HPEN hPen = CreatePen(PS_SOLID ,1 ,color );
	HPEN hOldPen = (HPEN )SelectObject(hdc ,hPen );

	Ellipse(hdc, (int)(pos.x - radius), (int)(pos.y - radius), (int)(pos.x + radius), (int)(pos.y + radius));

	SelectObject(hdc, hOldBrush);
	SelectObject(hdc, hOldPen);

	DeleteObject(hBrush);
	DeleteObject(hPen);

	return ;
}

void SOLAR::Update(double TimeElapsed )
{
	Gparameters._mass = mass ;
	Gparameters._pos = pos ;
	Gparameters._radius = radius ;
	for(size_t i=0 ;i<bombs.size() ;i++ )
	{
		bombs[i].Update(TimeElapsed );
		if(bombs[i].life <= 0 )
		{
			bombs.erase(bombs.begin()+i );
		}
	}
	for(size_t i=0 ;i<contrails.size() ;i++ )
	{
		contrails[i].life -- ;
		if(contrails[i].life <= 0 )
		{
			contrails.erase(contrails.begin()+i );
		}
	}
	for(size_t i=0 ;i<planets.size() ;i++ )
	{
		if(false )
		{
			PLANET__UPDATE__END__FLAG :
			continue ; 
		}
		double R = CalculateDistance(planets[i].pos ,pos );
		planets[i].Update(TimeElapsed ,Gparameters );
		if(R <= radius + planets[i].radius )
		{
			double angle = CalculateAngle(S2DVector(0 ,0 ),planets[i].velocity );
			S2DVector tmp(radius*cos(angle ),radius*sin(angle ));
			BOMB NewBomb(pos+tmp );
			NewBomb.color_type = 1 ;
			bombs.push_back(NewBomb );

			planets.erase(planets.begin()+i);
			goto PLANET__UPDATE__END__FLAG ;
		}
		if(AwayFromScreen(planets[i].pos ))
		{
			planets.erase(planets.begin()+i);
			goto PLANET__UPDATE__END__FLAG ;
		}
		if(planets[i].TimeToSpawnContrail <= 0 )
		{
			contrails.push_back(CONTRAIL(planets[i].pos ));
			planets[i].TimeToSpawnContrail = CONTRAIL__DEFAULT__INTERVAL ;
		}
		else
		{
			planets[i].TimeToSpawnContrail -- ;
		}
		for(size_t j=0 ;j<orbits.size() ;j++ )
		{
			if(fabs(orbits[j].radius - R ) < EPSILON )
			{
				for(size_t k=0 ;k<orbits[j].Surroundings.size() ;k++ )
				{
					if(CalculateDistance(planets[i].pos ,(orbits[j].Surroundings[k]).pos ) <= (planets[i].radius + (orbits[j].Surroundings)[k].radius) )
					{
						bombs.push_back(BOMB(planets[i].pos ,orbits[j].Surroundings[k].pos ));
						planets.erase(planets.begin()+i );
						orbits[j].DeletePlanet(k );
						goto PLANET__UPDATE__END__FLAG ;
					}
				}
			}
		}
		for(size_t j=i+1 ;j<planets.size() ;j++ )
		{
			if(CalculateDistance(planets[i].pos ,planets[j].pos ) < planets[i].radius + planets[j].radius )
			{
				bombs.push_back(BOMB(planets[i].pos ,planets[j].pos ));
				planets.erase(planets.begin()+j );
				planets.erase(planets.begin()+i );
				goto PLANET__UPDATE__END__FLAG ;
			}
		}

	}
	for(size_t i=0 ;i<orbits.size() ;i++ )
	{
		orbits[i].Update(TimeElapsed );
		if(orbits[i].Surroundings.size() <= 0 )
		{
			orbits.erase(orbits.begin()+i );
		}
	}
}

void SOLAR::ReadyToAddPlanet(double x ,double y )
{
	if(CalculateDistance(pos ,S2DVector(x ,y )) <= radius )return ;
	PLANET_SPAWN__X = x ;
	PLANET_SPAWN__Y = y ;
	PLANET_SPAWN__READY = true ;
	return ;
}

void SOLAR::AddPlanet(double x ,double y )
{
	if(!PLANET_SPAWN__READY )return ;
	if(planets.size() >= PLANET__DEFAULT__MAXIMUM )
	{
		planets.erase(planets.begin() );
	}
	PLANET tmpPlanet ;
	tmpPlanet.pos.x = PLANET_SPAWN__X ;
	tmpPlanet.pos.y = PLANET_SPAWN__Y ;
	S2DVector dpos(x-PLANET_SPAWN__X ,y-PLANET_SPAWN__Y );
	tmpPlanet.velocity = dpos*PLANET__DEFAULT__SPAWN__VELOCITY__SCALE ;
	planets.push_back(tmpPlanet );
	PLANET_SPAWN__READY = false ;
	return ;
}

void SOLAR::AddOrbit(double x ,double y )
{
	double R = CalculateDistance(pos ,S2DVector(x ,y )) ;
	if(CalculateDistance(pos ,S2DVector(x ,y )) <= radius )return ;

	PLANET tmpPlanet ;
	tmpPlanet.pos.x = x ;
	tmpPlanet.pos.y = y ;

	for(size_t i=0 ;i<orbits.size() ;i++ )
	{
		if(fabs(orbits[i].radius - R ) < EPSILON )
		{
			orbits[i].addPlanet(tmpPlanet );
			return ;
		}
	}
	ORBIT tmpOrbit(this ,R );
	tmpOrbit.speed = sqrt(G * mass / R );
	tmpOrbit.omega = ORBIT__DEFAULT__SLOW__SCALE * tmpOrbit.speed / R ;
	if(RandFloat() < 0.25 )tmpOrbit.omega = -tmpOrbit.omega ;
	tmpOrbit.addPlanet(tmpPlanet );
	orbits.push_back(tmpOrbit );
	return ;
}

double SOLAR::Mass(void )
{
	return mass ;
}

S2DVector SOLAR::position(void )
{
	return pos ;
}


double CalculateDistance(const S2DVector &p1 ,const S2DVector &p2 )
{
	double dx = p1.x - p2.x ;
	double dy = p1.y - p2.y ;
	return sqrt(dx*dx + dy*dy);
}

static bool AwayFromScreen(S2DVector pos )
{
	if(fabs(pos.y ) > 5*cyClient )
	{
		return true ;
	}
	if(fabs(pos.x ) > 5*cxClient )
	{
		return true ;
	}
	return false ;
}

void SpecialRender(HDC hdc )
{
	HBRUSH hBrush = CreateSolidBrush(PLANET__DEFAULT__COLOR );
	HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, hBrush);

	HPEN hPen = CreatePen(PS_SOLID ,1 ,PLANET__DEFAULT__COLOR );
	HPEN hOldPen = (HPEN)SelectObject(hdc ,hPen );

	MoveToEx(hdc ,(int)PLANET_SPAWN__X ,(int)PLANET_SPAWN__Y ,NULL );
	LineTo(hdc ,pxMouse ,pyMouse );

	Ellipse(hdc ,(int)(PLANET_SPAWN__X -PLANET__DEFAULT__RADIUS ),(int)(PLANET_SPAWN__Y -PLANET__DEFAULT__RADIUS ),
		(int)(PLANET_SPAWN__X +PLANET__DEFAULT__RADIUS ),(int)(PLANET_SPAWN__Y +PLANET__DEFAULT__RADIUS ));

	SelectObject(hdc ,hOldBrush );
	SelectObject(hdc ,hOldPen );

	DeleteObject(hBrush );
	DeleteObject(hPen );
}

void BombRender(HDC hdc ,const BOMB &bomb )
{
	int color_v ;
	COLORREF color ;
	switch(bomb.color_type )
	{
		case 0 :
		default :
		{
			color_v = 0xFF * bomb.life / BOMB__DEFAULT__LIFE ;
			color = RGB(color_v ,color_v ,color_v );
			break ;
		}
		case 1 :
		{
			color_v = 0xFF * bomb.life / BOMB__DEFAULT__LIFE ;
			color = RGB(color_v ,0 ,0 );
			break ;
		}
	}
	for(int i=0 ;i<BOMB__DEFAULT__PARTICLE__NUM ;i++ )
	{
		SetPixel(hdc ,(int)(bomb.particles[i].p.x ),(int)(bomb.particles[i].p.y ),color );
	}
}

BOMB::BOMB(const S2DVector _p1 ,const S2DVector _p2 ):p1(_p1 ),p2(_p2 )
{
	radius = BOMB__DEFAULT__RADIUS ;
	life = BOMB__DEFAULT__LIFE ;
	color_type = 0 ;
	S2DVector SpawnPos = 0.5*( _p1 + _p2 ) ;
	for(int i=0 ;i<BOMB__DEFAULT__PARTICLE__NUM ;i++ )
	{
		double angle = RandFloat() * (2*PI) ;
		double velocity = RandFloat() * BOME__DEFAULT__PARTICLE__MAX__VELOCITY ;
		particles[i].p = SpawnPos ;
		particles[i].v = S2DVector(velocity * cos(angle ) ,velocity * sin(angle ));
	}
}

BOMB::BOMB(const S2DVector _p1 ):p1(_p1 ),p2(_p1 )
{
	radius = BOMB__DEFAULT__RADIUS ;
	life = BOMB__DEFAULT__LIFE ;
	color_type = 0 ;
	S2DVector SpawnPos = _p1 ;
	for(int i=0 ;i<BOMB__DEFAULT__PARTICLE__NUM ;i++ )
	{
		double angle = RandFloat() * (2*PI) ;
		double velocity = RandFloat() * BOME__DEFAULT__PARTICLE__MAX__VELOCITY ;
		particles[i].p = SpawnPos ;
		particles[i].v = S2DVector(velocity * cos(angle ) ,velocity * sin(angle ));
	}
}

void BOMB::Update(double TimeElapsed )
{
	life -- ;
	for(int i=0 ;i<BOMB__DEFAULT__PARTICLE__NUM ;i++ )
	{
		particles[i].p += particles[i].v ;
	}
}