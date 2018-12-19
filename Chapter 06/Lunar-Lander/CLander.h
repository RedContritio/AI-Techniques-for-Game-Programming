#ifndef __CLANDER__DEF
#define __CLANDER__DEF
#include <windows.h>
#include <vector>
using std::vector ;

#include "defines.h"
#include "S2DVector.h"
#include "C2DMatrix.h"
#include "utils.h"

class CLander
{
	private :
		S2DVector pos ;
		double angle ;
		double scale ;
		double mass ;
		S2DVector velocity ;
		S2DVector PadPos ;
		vector<SPoint> vecShipVB ;
		vector<SPoint> vecShipVBTrans ;
		vector<SPoint> vecJetVB ;
		vector<SPoint> vecJetVBTrans ;
		bool JetOn ;
		bool isLanded ;
		bool LandedOk ;
		bool TestForImpact(vector<SPoint> & );
		void WorldTransform(vector<SPoint> & );
		bool CheckLandStatus(void );
	public :
		CLander(S2DVector ,S2DVector ,double );
		~CLander(void );
		void Render(HDC & );
		void Reset(S2DVector & );
		void UpdateShip(double );

};
#endif