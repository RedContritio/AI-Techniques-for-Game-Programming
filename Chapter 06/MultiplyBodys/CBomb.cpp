#include "CBomb.h"
#include "S2DMatrix.h"
BOMB::BOMB(const S2DVector &_p1 ,const S2DVector &_p2 ):p1(_p1 ),p2(_p2 ) //abandoned
{
	pos = (_p1+_p2)/2 ;
	double radius = (p1-p2).Length() ;
	life = BOMB__DEFAULT__LIFE ;
	colors.push_back(COLORUN(STAR__DEFAULT__COLOR ));
	colors.push_back(COLORUN(PLANET__DEFAULT__COLOR ));

	for(size_t i=0 ;i<200 ;i++ )
	{
		PARTICLE tmp ;
		double angle1 = RandFloat() * 2 * PI ;
		double R = radius * RandFloat() ;
		tmp.p = pos+S2DVector(R * cos(angle1 ) ,R * sin(angle1 ))  ;
		double angle2 = RandFloat() * 2 * PI ;
		double speed = 3 * RandFloat() * BOME__DEFAULT__PARTICLE__MAX__VELOCITY ;
		tmp.v = S2DVector(speed * cos(angle2 ),speed * sin(angle2 ));
		tmp.colortype = 0 ;
		particles.push_back(tmp);
	}
}

BOMB::BOMB(const S2DVector &_p1 ,const S2DVector &_p2 ,__MBS__TYPE _type1 ,__MBS__TYPE _type2 )
		  :p1(_p1 ),p2(_p2 ),type1(_type1 ),type2(_type2 )
{
	pos = (_p1+_p2)/2 ;
	double radius = (p1-p2).Length() ;
	double MaxSpeed = 3 * BOME__DEFAULT__PARTICLE__MAX__VELOCITY ;
	size_t NumOfLateral = 30 ;
	size_t NumOfRandom = 20 ;
	switch(type1 | type2 )
	{
		case __MBS__TYPE__STAR | __MBS__TYPE__STAR :
		{
			life = tlife = 10 * BOMB__DEFAULT__LIFE ;
			MaxSpeed *= 5 ;
			NumOfLateral *= 4 ;
			NumOfRandom *= 3 ;
			break ;
		}
		case __MBS__TYPE__STAR | __MBS__TYPE__PLANET :
		{
			life = tlife = 3 * BOMB__DEFAULT__LIFE ;
			MaxSpeed *= 1 ;
			NumOfLateral *= 2 ;
			NumOfRandom *= 2 ;
			break ;
		}
		case __MBS__TYPE__PLANET | __MBS__TYPE__PLANET :
		{
			life = tlife = BOMB__DEFAULT__LIFE ;
			MaxSpeed *= 0.5 ;
			NumOfLateral *= 1 ;
			NumOfRandom *= 1 ;
			break ;
		}
	}

	switch(_type1 )
	{
		case __MBS__TYPE__STAR :
		default :
		{
			colors.push_back(COLORUN(STAR__DEFAULT__COLOR ));
			break ;
		}
		case __MBS__TYPE__PLANET :
		{
			colors.push_back(COLORUN(PLANET__DEFAULT__COLOR ));
			break ;
		}
	}
	switch(_type2 )
	{
		case __MBS__TYPE__STAR :
		default :
		{
			colors.push_back(COLORUN(STAR__DEFAULT__COLOR ));
			break ;
		}
		case __MBS__TYPE__PLANET :
		{
			colors.push_back(COLORUN(PLANET__DEFAULT__COLOR ));
			break ;
		}
	}
	
	S2DVector dif = p1 - p2 ;
	dif.Normalize() ;
	for(size_t i=0 ;i<NumOfLateral ;i++ )
	{
		PARTICLE tmp ;
		C2DMATRIX a(dif.x ,dif.y );
		double angle = 90 ;
		if(RandInt(0 ,1 ))angle = -angle ;
		angle += RandFloat(-20 ,20 );
		C2DMATRIX rotation ;
		rotation.Rotate(angle/180*PI );
		a *= rotation ;
		tmp.p = pos ;
		tmp.v.x = a.x() ;
		tmp.v.y = a.y() ;
		double speed = RandFloat() * MaxSpeed ;
		tmp.v *= speed ;
		tmp.colortype = RandInt(0 ,1 );
		particles.push_back(tmp);
	}
	for(size_t i=0 ;i<NumOfRandom ;i++ )
	{
		PARTICLE tmp ;
		double angle1 = RandFloat() * 2 * PI ;
		double R = radius * RandFloat() ;
		tmp.p = pos+S2DVector(R * cos(angle1 ) ,R * sin(angle1 ))  ;
		double angle2 = RandFloat() * 2 * PI ;
		double speed = RandFloat() * MaxSpeed ;
		tmp.v = S2DVector(speed * cos(angle2 ),speed * sin(angle2 ));
		tmp.colortype = 0 ;
		particles.push_back(tmp);
	}
}

BOMB::~BOMB(void )
{
	;
}

bool BOMB::Exist(void )
{
	return life>=0;
}

void BOMB::Update(double TimeElapsed )
{
	for(size_t i=0 ;i<particles.size() ;i++ )
	{
		particles[i].p += particles[i].v ;
	}
	life -- ;
}

void BOMB::Render(HDC hdc )
{
	vector<COLORUN> colorTrans ;
	for(size_t i=0 ;i<colors.size() ;i++ )
	{
		COLORUN tmp = colors[i] ;
		tmp.c.r = (int)(1.0 * life / tlife * (colors[i].c.r ));
		tmp.c.g = (int)(1.0 * life / tlife * (colors[i].c.g ));
		tmp.c.b = (int)(1.0 * life / tlife * (colors[i].c.b ));
		colorTrans.push_back(tmp);
	}
	for(size_t i=0 ;i<particles.size() ;i++ )
	{
		SetPixel(hdc ,(int)(particles[i].p.x ),(int)(particles[i].p.y ),colorTrans[particles[i].colortype].v );
	}
}