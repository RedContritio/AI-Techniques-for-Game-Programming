#ifndef __CBODY__INC
#define __CBODY__INC
#include <windows.h>
#include <vector>
using std::vector ;
#include "S2DVector.h"
#include "defines.h"

extern int cxClient ,cyClient ;

class CONTROLLER ;

union COLORUN
{
	struct 
	{
		BYTE r ;
		BYTE g ;
		BYTE b ;
		BYTE a ;
	}c ; //conponents
	COLORREF v ;
	COLORUN(void );
	COLORUN(const unsigned long & );
	COLORUN& operator = (const unsigned long & );
};

struct CONTRAIL
{
	S2DVector p ;
	int life ;
	int tlife ;
	CONTRAIL(const S2DVector& ,int );
	void Update(void );
	void Render(HDC ,COLORUN )const ;
};

class BODY
{
	protected :
		S2DVector pos ;
		S2DVector velocity ;
		S2DVector accelator ;
		double mass ;
		double radius ;
		COLORUN color ;
		vector<CONTRAIL> contrails ;
		void SetAccelator(const S2DVector & );
	public :
		BODY(const S2DVector & ,const S2DVector & );
		BODY(const S2DVector & ,const S2DVector & ,double ,double ,COLORUN );
		~BODY(void );
		virtual void Update(double ) = 0 ;
		virtual void Render(HDC ) = 0 ;
		virtual void RenderVelocity(HDC ) = 0 ;
		bool OutOfWorld(void ) ;
		friend class CONTROLLER ;
};
#endif