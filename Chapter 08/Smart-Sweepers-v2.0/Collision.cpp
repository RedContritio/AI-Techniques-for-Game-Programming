#include "Collision.h"

bool LineIntersection2D(const SPoint A, const SPoint B, const SPoint C, const SPoint D, double & dist)
{
	double rTop =(A.y -C.y )*(D.x -C.x )-(A.x -C.x )*(D.y -C.y );
	double rBot =(B.x -A.x )*(D.y -C.y )-(B.y -A.y )*(D.x -C.x );
	double sTop =(A.y -C.y )*(B.x -A.x )-(A.x -C.x )*(B.y -A.y );
	double sBot =(B.x -A.x )*(D.y -C.y )-(B.y -A.y )*(D.x -C.x );
	if(rBot == 0 || sBot == 0 )
	{
		return false ;
	}
	double r =rTop /rBot ;
	double s =sTop /sBot ;
	if((r >0 )&&(r <1.0 )&&(s >0 )&&(s <1.0 ))
	{
		dist =r ;
		return true ;
	}
	else
	{
		dist =0 ;
		return false;
	}
}
