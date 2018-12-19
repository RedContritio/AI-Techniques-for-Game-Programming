#ifndef __CPLANET__INC
#define __CPLANET__INC
#include "CBody.h"

class CONTROLLER ;

class PLANET : public BODY
{
	public :
		PLANET(void );
		PLANET(double ,double );
		PLANET(const S2DVector & );
		PLANET(const S2DVector & ,const S2DVector & );
		~PLANET(void );
		void Update(double );
		void Render(HDC );
		void RenderVelocity(HDC );
		friend class CONTROLLER ;
};

void SpecialPlanetRender(HDC ,const S2DVector & ,const S2DVector & );
#endif