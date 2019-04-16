#include "CLander.h"
#pragma warning( disable:4996)
const int NumLanderVerts = 30;

const SPoint LanderShape[NumLanderVerts] = 
{//body of lander
 SPoint(-1 ,0 ),SPoint(1 ,0 ),SPoint(1 ,-0.5 ),SPoint(-1 ,-0.5 ),
 //top of lander
 SPoint(-0.5 ,0 ),SPoint(-1 ,0.3 ),SPoint(-1 ,0.7 ),SPoint(-0.5 ,1 ),
 SPoint(0.5 ,1 ),SPoint(1 ,0.7 ),SPoint(1 ,0.3 ),SPoint(0.5 ,0 ),
 //legs
 SPoint(-1 ,-0.4 ),SPoint(-1.3 ,-0.8 ),SPoint(-1.3 ,-1.2 ),SPoint(-1.5 ,-1.2 ),
 SPoint(-1.1 ,-1.2 ),SPoint(-0.9 ,-0.5 ),SPoint(-1.3, -0.8),

 SPoint(1 ,-0.4 ),SPoint(1.3 ,-0.8 ),SPoint(1.3 ,-1.2 ),SPoint(1.5 ,-1.2 ),
 SPoint(1.1 ,-1.2 ),SPoint(0.9 ,-0.5 ),SPoint(1.3 ,-0.8 ),
 //rocket
 SPoint(-0.2 ,-0.5 ),SPoint(-0.3 ,-0.8 ),SPoint(0.3 ,-0.8 ),SPoint(0.2 ,-0.5 )};

const int NumJetVerts = 5;

const SPoint JetShape[NumJetVerts] = 
{
 SPoint(-0.1 ,-0.9 ),SPoint(-0.2 ,-1.2 ),SPoint(0 ,-1.6 ),SPoint(0.2 ,-1.2 ),
 SPoint(0.1 ,-0.9 )};

extern int cxClient ,cyClient ;

CLander::CLander(S2DVector _pos ,S2DVector _padpos ,double rotation )
:pos(_pos ),angle(rotation ),scale(__LANDER__SCALE ),mass(SHIP__MASS ),velocity(S2DVector(0 ,0 )),
PadPos(_padpos ),isLanded(false )
{
	int i ; // fuck VC++
	for(i=0 ;i<NumLanderVerts ;i++ )
	{
		vecShipVB.push_back(LanderShape[i] );
	}
	for(i=0 ;i<NumJetVerts ;i++ )
	{
		vecJetVB.push_back(JetShape[i] );
	}
	vecShipVBTrans = vecShipVB ;
	vecJetVBTrans = vecJetVB ;
}

CLander::~CLander(void )
{
	;
}

#define __VSVBT vecShipVBTrans
void CLander::Render(HDC &hdc )
{
	int i ; // fuck VC++
	//body renderer
	MoveToEx(hdc ,(int)__VSVBT[4-1].x ,(int)__VSVBT[4-1].y ,NULL );
	for(i=0 ;i<4 ;i++ )
	{
		LineTo(hdc ,(int)__VSVBT[i].x ,(int)__VSVBT[i].y );
	}
	//top renderer
	MoveToEx(hdc ,(int)__VSVBT[4].x ,(int)__VSVBT[4].y ,NULL );
	for(i=5 ;i<12 ;i++ )
	{
		LineTo(hdc ,(int)__VSVBT[i].x ,(int)__VSVBT[i].y );
	}

	// legs renderer
	for(i=0 ;i<2 ;i++ )
	{
		MoveToEx(hdc ,(int)__VSVBT[12+7*i].x ,(int)__VSVBT[12+7*i].y ,NULL );
		LineTo(hdc ,(int)__VSVBT[13+7*i].x ,(int)__VSVBT[13+7*i].y );
		LineTo(hdc ,(int)__VSVBT[14+7*i].x ,(int)__VSVBT[14+7*i].y );
		
		for(int j=0 ;j<2 ;j++ )
		{
			MoveToEx(hdc ,(int)__VSVBT[15+7*i+2*j].x ,(int)__VSVBT[15+7*i+2*j].y ,NULL );
			LineTo(hdc ,(int)__VSVBT[16+7*i+2*j].x ,(int)__VSVBT[16+7*i+2*j].y );
		}
	}

	// burner renderer
	MoveToEx(hdc ,(int)__VSVBT[26].x ,(int)__VSVBT[26].y ,NULL );
	for(i=27 ;i<30 ;i++ )
	{
		LineTo(hdc ,(int)__VSVBT[i].x ,(int)__VSVBT[i].y );
	}

	if(JetOn )
	{
		vecJetVBTrans = vecJetVB ;
		WorldTransform(vecJetVBTrans );
		MoveToEx(hdc ,(int)vecJetVBTrans[0].x ,(int)vecJetVBTrans[0].y ,NULL );
		for(i=1 ;i<(int)(vecJetVBTrans.size()) ;i++ )
		{
			LineTo(hdc ,(int)vecJetVBTrans[i].x ,(int)vecJetVBTrans[i].y );
		}
	}

	if(isLanded )
	{
		char s[50] = "";
		if(LandedOk )
		{
			strcat(s ,"Great Landing !");
		}
		else
		{
			strcat(s ,"Crashed !");
		}
		int len = strlen(s );
		TextOut(hdc ,(cxClient-4*len)/2 ,160 ,s ,len );
	}
	return ;
}
#undef __VSVBT

void CLander::Reset(S2DVector &_padpos )
{
	pos = S2DVector(cxClient/2 ,cyClient-50.0 );
	angle = PI ;
	velocity = S2DVector(0 ,0 );
	PadPos = _padpos ;
	isLanded = false ;
	LandedOk = false ;
	return ;
}

void CLander::UpdateShip(double TimeElapsed )
{
	if( isLanded )
	{
		return ;
	}
	JetOn = false ;

	if(KEYDOWN(VK_SPACE ))
	{
		double ShipAcceleration = THRUSH__PER__SECOND * TimeElapsed / mass ;
		velocity.x -= ShipAcceleration * sin(angle );
		velocity.y += ShipAcceleration * cos(angle );
		JetOn = true ;
	}

	if(KEYDOWN(VK_LEFT ))
	{
		angle -= ROTATION__PER__SECOND * TimeElapsed ;
		if(angle < -PI )
		{
			angle -= 2*PI ;
		}
	}
	if(KEYDOWN(VK_RIGHT ))
	{
		angle += ROTATION__PER__SECOND * TimeElapsed ;
		if(angle > 2*PI )
		{
			angle += 2*PI ;
		}
	}

	velocity.y += GRAVITY * TimeElapsed ;
	pos += velocity * ( TimeElapsed * SCALING_FACTOR ) ;
	if(pos.x > cxClient )
	{
		pos.x = 0 ;
	}
	if(pos.x < 0 )
	{
		pos.x = cxClient ;
	}

	vecShipVBTrans = vecShipVB ;
	WorldTransform(vecShipVBTrans );

	if(TestForImpact(vecShipVBTrans ))
	{
		if(!isLanded )
		{
			if(CheckLandStatus() )
			{
				LandedOk = true ;
			}
			isLanded = true ;
		}
	}
}

void CLander::WorldTransform(vector<SPoint> &vecPoints )
{
	C2DMATRIX TransMatrix ;
	TransMatrix.Scale(scale ,scale );

	{
		C2DMATRIX RotationMatrix ;
		RotationMatrix.Rotate(angle );
		TransMatrix *= RotationMatrix ;
	}

	{
		C2DMATRIX MoveMatrix ;
		MoveMatrix.Move(pos.x ,pos.y );
		TransMatrix *= MoveMatrix ;
	}

	for(int i=0 ;i<(int)(vecPoints.size()) ;i++ )
	{
		C2DMATRIX tmp(vecPoints[i].x ,vecPoints[i].y );
		tmp *= TransMatrix ;
		vecPoints[i].x = tmp.x() ;
		vecPoints[i].y = tmp.y() ;
	}
	return ;
}

bool CLander::TestForImpact(vector<SPoint> &points )
{
	const int GroundLevel = 50 + 5 ;
	for(int i=0 ;i<(int)(points.size()) ;i++ )
	{
		if(points[i].y < GroundLevel )
		{
			return true ;
		}
	}
	return false ;
}

bool CLander::CheckLandStatus(void )
{
	double DisX = fabs(PadPos.x - pos.x );
	double Speed = sqrt(velocity.x * velocity.x + velocity.y * velocity.y );
	if((DisX < __TOLERANCE__DIST )&&(Speed < __TOLERANCE__SPEED )&&(fabs(angle) < __TOLERANCE__ANGLE))
	{
		return true ;
	}
	return false ;
}