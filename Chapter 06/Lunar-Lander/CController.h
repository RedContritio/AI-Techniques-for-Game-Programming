#ifndef __CCTRLLER__DEF
#define __CCTRLLER__DEF

#include <windows.h>
#include <vector>
using std::vector ;

#include "S2DVector.h"
#include "CLander.h"
#include "defines.h"
#include "utils.h"

extern int cxClient ,cyClient ;

class CController
{
	private :
		CLander *lander ;
		bool Succeeded ;
		vector<SPoint> vecStarsVB ;
		vector<SPoint> vecPadVB ;
		S2DVector PadPos ;
		void RenderLandingPad(HDC & );
	public :
		CController(void );
		~CController(void );
		bool Update(double );
		void NewRun(void );
		void Render(HDC & );
		void Resize(void );
};
#endif