#ifndef __CBOMB__INC
#define __CBOMB__INC
#include <windows.h>
#include <vector>
using std::vector ;

#include "LogSpawner.h"
using RedContritio::LOGSPAWNER ;
extern LOGSPAWNER loger ;

#include "S2DVector.h"

#include "defines.h"
#include "utils.hpp"

#include "CStar.h"
struct PARTICLE
{
	S2DVector p ;
	S2DVector v ;
	int colortype ;
};

class BOMB
{
	private :
		vector<PARTICLE> particles ;
		vector<COLORUN> colors ;
		S2DVector pos ;
		int tlife ;
		int life ;
		S2DVector p1 ;
		__MBS__TYPE type1 ;
		S2DVector p2 ;
		__MBS__TYPE type2 ;
	public :
		BOMB(const S2DVector & ,const S2DVector & );
		BOMB(const S2DVector & ,const S2DVector & ,__MBS__TYPE ,__MBS__TYPE );
		~BOMB(void );
		void Update(double );
		void Render(HDC );
		bool Exist(void );
};

#endif