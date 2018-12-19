#include "CController.h"
extern int cxClient ,cyClient ;


CONTROLLER::CONTROLLER(void )
{
	isPaused = false ;
	isWatching = false ;
}

CONTROLLER::~CONTROLLER(void )
{
	;
}

void CONTROLLER::Update(double TimeElapsed )
{
	if(isPaused )return ;
	for(size_t i=0 ;i<stars.size() ;i++ )
	{
		if(stars[i].OutOfWorld() )
		{
			stars.erase(stars.begin() + i );
			i-- ;
			continue ;
		}
		S2DVector a = CalculateStarAccelator(i );
		stars[i].SetAccelator(a );
		stars[i].Update(TimeElapsed );
	}
	CheckCrash(__MBS__TYPE__STAR ,__MBS__TYPE__STAR );
	for(size_t i=0 ;i<planets.size() ;i++ )
	{
		if(planets[i].OutOfWorld() )
		{
			planets.erase(planets.begin() + i );
			i-- ;
			continue ;
		}
		S2DVector a = CalculatePlanetAccelator(i );
		planets[i].SetAccelator(a );
		planets[i].Update(TimeElapsed );
	}
	CheckCrash(__MBS__TYPE__STAR ,__MBS__TYPE__PLANET );
	CheckCrash(__MBS__TYPE__PLANET ,__MBS__TYPE__PLANET );
	for(size_t i=0 ;i<bombs.size() ;i++ )
	{
		bombs[i].Update(TimeElapsed );
		if(!(bombs[i].Exist()))
		{
			bombs.erase(bombs.begin() + i );
		}
	}
}

void CONTROLLER::Render(HDC hdc )
{
	if(isLButtonDown )
	{
		SpecialStarRender(hdc ,MouseDown ,MousePos );
	}
	if(isRButtonDown )
	{
		SpecialPlanetRender(hdc ,MouseDown ,MousePos );
	}

	for(size_t i=0 ;i<bombs.size() ;i++ )
	{
		bombs[i].Render(hdc );
	}

	if(isPaused || isWatching )
	{
		for(size_t i=0 ;i<planets.size() ;i++ )
		{
			planets[i].RenderVelocity(hdc );
		}
		for(size_t i=0 ;i<stars.size() ;i++ )
		{
			stars[i].RenderVelocity(hdc );
		}
	}
	for(size_t i=0 ;i<planets.size() ;i++ )
	{
		planets[i].Render(hdc );
	}
	for(size_t i=0 ;i<stars.size() ;i++ )
	{
		stars[i].Render(hdc );
	}
}

void CONTROLLER::LButtonDown(double x ,double y )
{
	if(isRButtonDown )return ;
	MouseDown.x = x ;
	MouseDown.y = y ;
	isLButtonDown = true ;
	size_t tid = 0 ;
	if((tid = FindExistingBody(__MBS__TYPE__STAR ,MouseDown )) < stars.size() )
	{
		MotionlessAdjust(__MBS__TYPE__STAR ,tid );
	}
	return ;
}

void CONTROLLER::LButtonUp(double x ,double y )
{
	if(isRButtonDown )return ;
	if(!isLButtonDown )return ;
	MouseUp.x = x ;
	MouseUp.y = y ;
	if(isLMLA )
	{
		STAR tmp = StarKeeper.Load() ;
		S2DVector vp = MousePos - tmp.pos ;
		vp *= dvScale ;
		tmp.velocity = vp ;
		stars.push_back(tmp );
		isPaused = PauseStatus.Load() ;
		isLMLA = false ;
	}
	else
	{
		S2DVector vp = MouseUp - MouseDown ;
		vp *= dvScale ;
		stars.push_back(STAR(MouseDown ,vp ));
	}
	isLButtonDown = false ;
}

void CONTROLLER::RButtonDown(double x ,double y )
{
	if(isLButtonDown )return ;
	MouseDown.x = x ;
	MouseDown.y = y ;
	isRButtonDown = true ;
	size_t tid = 0 ;
	if((tid = FindExistingBody(__MBS__TYPE__PLANET ,MouseDown )) < planets.size() )
	{
		MotionlessAdjust(__MBS__TYPE__PLANET ,tid );
	}
	return ;
}

void CONTROLLER::RButtonUp(double x ,double y )
{
	if(isLButtonDown )return ;
	if(!isRButtonDown )return ;
	MouseUp.x = x ;
	MouseUp.y = y ;
	if(isRMLA )
	{
		PLANET tmp = PlanetKeeper.Load() ;
		S2DVector vp = MousePos - tmp.pos ;
		vp *= dvScale ;
		tmp.velocity = vp ;
		planets.push_back(tmp );
		isPaused = PauseStatus.Load() ;
		isRMLA = false ;
	}
	else
	{
		S2DVector vp = MouseUp - MouseDown ;
		vp *= dvScale ;
		planets.push_back(PLANET(MouseDown ,vp ));
	}
	isRButtonDown = false ;
}

void CONTROLLER::MouseMove(double x ,double y )
{
	MousePos.x = x ;
	MousePos.y = y ;
}

void CONTROLLER::Pause(void )
{
	isPaused = !isPaused ;
}

void CONTROLLER::Watch(void )
{
	isWatching = !isWatching ;
}

void CONTROLLER::MotionlessAdjust(__MBS__TYPE type ,size_t id )
{
	switch(type )
	{
		case __MBS__TYPE__STAR :
		{
			PauseStatus.Save(isPaused ) ;
			StarKeeper.Save(stars[id] );

			isLMLA = true ;
			isPaused = true ;

			MouseDown = StarKeeper.Load().pos ;
			stars.erase(stars.begin()+id );
			break ;
		}
		case __MBS__TYPE__PLANET :
		{
			PauseStatus.Save(isPaused ) ;
			PlanetKeeper.Save(planets[id] );

			isRMLA = true ;
			isPaused = true ;

			MouseDown = PlanetKeeper.Load().pos ;
			planets.erase(planets.begin()+id );
			break ;
		}
		default :
		{
			;
		}
	}
}

S2DVector CONTROLLER::CalculateStarAccelator(size_t id )const
{
	S2DVector a ;
	S2DVector tmp ;
	double R = 0 ;
	for(size_t i=0 ;i<id ;i++ )
	{
		tmp = stars[i].pos - stars[id].pos ;
		R = tmp.Length() ;
		if(R < EPSILON )continue ;
		tmp.Normalize() ;
		a += ((G*stars[i].mass) / (R*R)) * tmp ;
	}
	for(size_t i=id+1 ;i<stars.size() ;i++ )
	{
		tmp = stars[i].pos - stars[id].pos ;
		R = tmp.Length() ;
		if(R < EPSILON )continue ;
		tmp.Normalize() ;
		a += ((G*stars[i].mass) / (R*R)) * tmp ;
	}
	return a ;
}

S2DVector CONTROLLER::CalculatePlanetAccelator(size_t id )const
{
	S2DVector a ;
	S2DVector tmp ;
	double R = 0 ;
	for(size_t i=0 ;i<stars.size() ;i++ )
	{
		tmp = stars[i].pos - planets[id].pos ;
		R = tmp.Length() ;
		if(R < EPSILON )continue ;
		tmp.Normalize() ;
		a += ((G*stars[i].mass) / (R*R)) * tmp ;
	}
	return a ;
}

void CONTROLLER::CheckCrash(__MBS__TYPE type1 ,__MBS__TYPE type2 )
{
	__MBS__TYPE aim = type1 | type2 ;
	switch(aim )
	{
		case __MBS__TYPE__STAR | __MBS__TYPE__STAR :
		{
			for(size_t i=0 ;i<stars.size() ;i++ )
			{
				for(size_t j=i+1 ;j<stars.size() ;j++ )
				{
					if(CalculateDistance(stars[i].pos ,stars[j].pos ) <= (stars[i].radius + stars[j].radius ))
					{
						bombs.push_back(BOMB(stars[i].pos ,stars[j].pos ,__MBS__TYPE__STAR ,__MBS__TYPE__STAR ));
						stars.erase(stars.begin() + j );
						stars.erase(stars.begin() + i );
					}
				}
			}
			break ;
		}
		case __MBS__TYPE__STAR | __MBS__TYPE__PLANET :
		{
			for(size_t i=0 ;i<stars.size() ;i++ )
			{
				for(size_t j=0 ;j<planets.size() ;j++ )
				{
					if(CalculateDistance(stars[i].pos ,planets[j].pos ) <= (stars[i].radius + planets[j].radius ))
					{
						bombs.push_back(BOMB(stars[i].pos ,planets[j].pos ,__MBS__TYPE__STAR ,__MBS__TYPE__PLANET ));
						planets.erase(planets.begin() + j );
						// stars.erase(stars.begin() + i );
					}
				}
			}
			break ;
		}
		case __MBS__TYPE__PLANET | __MBS__TYPE__PLANET :
		{
			for(size_t i=0 ;i<planets.size() ;i++ )
			{
				for(size_t j=i+1 ;j<planets.size() ;j++ )
				{
					if(CalculateDistance(planets[i].pos ,planets[j].pos ) <= (planets[i].radius + planets[j].radius ))
					{
						bombs.push_back(BOMB(planets[i].pos ,planets[j].pos ,__MBS__TYPE__PLANET ,__MBS__TYPE__PLANET ));
						planets.erase(planets.begin() + j );
						planets.erase(planets.begin() + i );
					}
				}
			}
			break ;
		}
	}
}



bool CONTROLLER::OutOfWorld(const S2DVector &pos )
{
	if( fabs(pos.x ) > 5*cxClient )return true ;
	if( fabs(pos.y ) > 5*cyClient )return true ;
	return false ;
}

size_t CONTROLLER::FindExistingBody(__MBS__TYPE type ,const S2DVector &mpos )const 
{
	size_t i=0  ;
	switch(type )
	{
		case __MBS__TYPE__STAR :
		{
			for(i=0 ;i<stars.size() ;i++ )
			{
				if(CalculateDistance(stars[i].pos ,mpos ) <= 2*stars[i].radius )
				{
					return i ;
				}
			}
			return stars.size() ;
			break ;
		}
		case __MBS__TYPE__PLANET :
		{
			for(i=0 ;i<planets.size() ;i++ )
			{
				if(CalculateDistance(planets[i].pos ,mpos ) <= 2*planets[i].radius )
				{
					return i ;
				}
			}
			return planets.size() ;
			break ;
		}
		default :
		{
			return (size_t )(-1 );
		}
	}
}
