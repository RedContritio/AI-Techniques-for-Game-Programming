#ifndef __CCONTROLLER__INC
#define __CCONTROLLER__INC
#include <windows.h>
#include "utils.hpp"

#include "LogSpawner.h"
#include "StatusKeeper.hpp"
using RedContritio::LOGSPAWNER ;
using RedContritio::StatusKeeper ;

extern LOGSPAWNER loger ;

#include <vector>
using std::vector ;

#include "CStar.h"
#include "CPlanet.h"
#include "CBomb.h"

class CONTROLLER
{
	private :
		vector<STAR> stars ;
		vector<PLANET> planets ;
		vector<BOMB> bombs ;
		S2DVector MouseDown ;
		S2DVector MouseUp ;
		S2DVector MousePos ;
		bool isLButtonDown ;
		bool isRButtonDown ;
		bool isPaused ;
		bool isWatching ;
		StatusKeeper<bool> PauseStatus ;
		StatusKeeper<STAR> StarKeeper ;
		StatusKeeper<PLANET> PlanetKeeper ;
		bool isLMLA ; // MotionlessAdjust
		bool isRMLA ; // Planet MLA
		S2DVector CalculateStarAccelator(size_t )const ;
		S2DVector CalculatePlanetAccelator(size_t )const ;
		void CheckCrash(__MBS__TYPE ,__MBS__TYPE );
		bool OutOfWorld(const S2DVector & );
		size_t FindExistingBody(__MBS__TYPE ,const S2DVector & )const ;
		void MotionlessAdjust(__MBS__TYPE ,size_t );
	public :
		CONTROLLER(void );
		~CONTROLLER(void );
		void Update(double );
		void Render(HDC );

		void LButtonDown(double ,double );
		void LButtonUp(double ,double );
		void RButtonDown(double ,double );
		void RButtonUp(double ,double );
		void MouseMove(double ,double );
		void Pause(void );
		void Watch(void );
};

void SpecialStarRender(HDC ,const S2DVector & ,const S2DVector & );
void StarVelocityRender(HDC ,const S2DVector & ,const S2DVector & );
#endif
