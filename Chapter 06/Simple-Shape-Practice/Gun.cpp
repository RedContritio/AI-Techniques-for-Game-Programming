#include "Gun.h"
#pragma warning( disable:4996)
const int NumGunVerts = 8 ;
const SPoint gunPoint[NumGunVerts] = {SPoint(2 ,1 ),SPoint(2 ,-1 ),SPoint(-2 ,-1 ),SPoint(-2 ,1 ),
							 SPoint(-1 ,1 ),SPoint(-1 ,2 ),SPoint(1 ,2 ),SPoint(1 ,1 ) };
SPoint::SPoint(double _x = 0 ,double _y = 0 ):x(_x ),y(_y )
{
	;
}

GUN::GUN(double x ,double y ,double scale ,double rotation )
:dPos(x ,y ),Scale(scale ),Rotation(rotation )
{
	for(int i=0 ;i<NumGunVerts ;i++ )
	{
		vecGunVB.push_back(gunPoint[i] );
	}
}

void GUN::WorldTransform(void )
{
	vecGunVBTrans = vecGunVB ;
	int i ; // fuck VC++
	for(i=0 ;i<NumGunVerts ;i++ )
	{
		vecGunVBTrans[i].x = vecGunVB[i].x * cos(Rotation ) - vecGunVB[i].y * sin(Rotation );
		vecGunVBTrans[i].y = vecGunVB[i].x * sin(Rotation ) + vecGunVB[i].y * cos(Rotation );
	}
	for(i=0 ;i<NumGunVerts ;i++ )
	{
		vecGunVBTrans[i].x *= Scale ;
		vecGunVBTrans[i].y *= Scale ;
	}
	for(i=0 ;i<NumGunVerts ;i++ )
	{
		vecGunVBTrans[i].x += dPos.x ;
		vecGunVBTrans[i].y += dPos.y ;
	}
}

void GUN::Render(HDC hdc )
{
	WorldTransform( );
	MoveToEx(hdc, (int)(vecGunVBTrans[NumGunVerts-1].x), (int)(vecGunVBTrans[NumGunVerts-1].y), NULL);
	for(int i=0 ;i<NumGunVerts ;i++ )
	{
		LineTo(hdc , (int)vecGunVBTrans[i].x, (int)vecGunVBTrans[i].y);
	}
	{
		char sdPos[20] ="";
		sprintf(sdPos ,"dPosition : (%g ,%g )" ,dPos.x ,dPos.y );
		TextOut(hdc ,5 ,0 ,sdPos ,strlen(sdPos ));
		char sRotation[20] ="";
		sprintf(sRotation ,"Rotation : %lf " ,Rotation );
		TextOut(hdc ,5 ,15 ,sRotation ,strlen(sRotation ));
		char sScale[20] ="";
		sprintf(sScale ,"Scale : %g " ,Scale );
		TextOut(hdc ,5 ,30 ,sScale ,strlen(sScale ));
	}
}

void GUN::Update(void )
{
	if(KEYDOWN(VK_LEFT ))
	{
		Rotation -= __MAX__ROTATION__PER__TICK ;
		if(Rotation < -PI )
		{
			Rotation += 2 * PI ;
		}
	}
	if(KEYDOWN(VK_RIGHT ))
	{
		Rotation += __MAX__ROTATION__PER__TICK ;
		if(Rotation > 2*PI )
		{
			Rotation -= 2 * PI ;
		}
	}


	if(KEYDOWN(VK_UP ))
	{
		Scale += __MAX__SCALE__PER__TICK ;
	}
	if(KEYDOWN(VK_DOWN ))
	{
		Scale -= __MAX__SCALE__PER__TICK ;
	}

	if(KEYDOWN('A' ))
	{
		dPos.x -= __MAX__TRANSLATION__PER__TICK ;
		if(dPos.x <  abs(Scale ) )
		{
			dPos.x = abs(Scale ) ;
		}
	}
	if(KEYDOWN('D' ))
	{
		dPos.x += __MAX__TRANSLATION__PER__TICK ;
		if(dPos.x > __WINDOW__WIDTH - abs(Scale ) )
		{
			dPos.x = __WINDOW__WIDTH - abs(Scale ) ;
		}
	}
	if(KEYDOWN('W' ))
	{
		dPos.y -= __MAX__TRANSLATION__PER__TICK ;
		if(dPos.y < abs(Scale ) )
		{
			dPos.y = abs(Scale ) ;
		}
	}
	if(KEYDOWN('S' ))
	{
		dPos.y += __MAX__TRANSLATION__PER__TICK ;
		if(dPos.y > __WINDOW__HEIGHT - abs(Scale ) )
		{
			dPos.y = __WINDOW__HEIGHT - abs(Scale ) ;
		}
	}
}