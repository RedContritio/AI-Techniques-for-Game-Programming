#ifndef __CSOLAR__DEF
#define __CSOLAR__DEF

#include <windows.h>
#include <vector>
using std::vector ;

#include "LogSpawner.h"
using RedContritio::LogSpawner ;

#include "S2DVector.h"
#include "S2DMatrix.h"

#include "utils.hpp"
#include "defines.h"

extern int cxClient ,cyClient ;
extern int pxMouse ,pyMouse ;
#ifdef DEBUG
extern LogSpawner loger ;
#endif


struct PLANET ;
struct ORBIT ;
class SOLAR ;

struct GravityVariables 
{
	double _mass ;
	S2DVector _pos ;
	double _radius ;
};

struct CONTRAIL 
{
	S2DVector pos ;
	int life ;
	CONTRAIL(double ,double );
	CONTRAIL(S2DVector );
};

struct PARTICLE
{
	S2DVector p ;
	S2DVector v ;
};

struct BOMB
{
	S2DVector p1 ;
	S2DVector p2 ;
	PARTICLE particles[BOMB__DEFAULT__PARTICLE__NUM];
	double radius ;
	int life ;
	int color_type ;
	BOMB(const S2DVector ,const S2DVector );
	BOMB(const S2DVector );
	void Update(double );
};

struct PLANET
{
	S2DVector pos ;
	double mass ;
	double density ;
	S2DVector velocity ;
	double radius ;
	double angle ;
	unsigned int color ;
	int TimeToSpawnContrail ;
	PLANET(void );
	~PLANET(void );
	void Update(double ,const GravityVariables& );
};

struct ORBIT
{
	SOLAR* CenterObject ;
	double radius ;
	double speed ;
	double omega ;
	vector<PLANET> Surroundings ;
	vector<double> angles ;
	ORBIT(SOLAR*, double);
	~ORBIT(void);
	void Update(double );
	void DeletePlanet(size_t id );
	double addPlanet(PLANET & );
};

class SOLAR
{
	private :
		S2DVector pos ;
		double mass ;
		double radius ;
		unsigned int color ;
		vector<PLANET> planets ;
		vector<ORBIT> orbits ;
		vector<CONTRAIL> contrails ;
		vector<BOMB> bombs ;
		void PushPlanet(const PLANET & );
	public :
		GravityVariables Gparameters ;// Whoever need these infomations could use it 
		SOLAR(void );
		SOLAR(S2DVector ,double ,double ,unsigned int );
		~SOLAR(void );
		S2DVector position(void );
		double Mass(void );
		void Render(HDC );
		void Update(double );
		void ReadyToAddPlanet(double ,double );
		void AddPlanet(double ,double );
		void AddOrbit(double ,double );
		friend struct ORBIT ;
		friend void PlanetRender(HDC ,const PLANET& );
		friend void OrbitRender(HDC ,const ORBIT& );
};



void PlanetRender(HDC ,const PLANET & );
void OrbitRender(HDC ,const ORBIT & );
void SpecialRender(HDC );
void BombRender(HDC ,const BOMB & );
inline void ContrailsRender(HDC hdc ,const CONTRAIL &contrail )
{
	int color_value = (int)((1.0* contrail.life/CONTRAIL__DEFAULT__MAX__LIFE) * 0x80 ) ;
	SetPixel(hdc ,(int)(contrail.pos.x ),(int)(contrail.pos.y ),RGB(color_value ,color_value ,color_value ));
	return ;
}

double CalculateAngle(const S2DVector & ,const S2DVector & ); // the 1st pos looks from the 2nd pos
double CalculateDistance(const S2DVector & ,const S2DVector & );
#endif