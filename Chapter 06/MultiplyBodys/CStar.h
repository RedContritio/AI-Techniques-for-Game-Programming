#ifndef __CSTAR__INC
#define __CSTAR__INC
#include "CBody.h"

class CONTROLLER ;

class STAR : public BODY
{
	public :
		STAR(void );
		STAR(double ,double );
		STAR(const S2DVector & );
		STAR(const S2DVector & ,const S2DVector & );
		~STAR(void );
		void Update(double );
		void Render(HDC );
		void RenderVelocity(HDC );
		friend class CONTROLLER ;
};
#endif