#include "CMineSweeper.h"

CMineSweeper::CMineSweeper(void ):mRotation(RandFloat() * CPARAMETERS::PI * 2 ),LTrack(0.16 ),RTrack(0.16 ),
								  mFitness(0 ),mScale(CPARAMETERS::SweeperScale ),ClosestMine(0 )
{
	mPosition = S2DVector(RandFloat() * CPARAMETERS::WindowWidth ,RandFloat() * CPARAMETERS::WindowHeight );
}

void CMineSweeper::Reset(void )
{
	mPosition = S2DVector(RandFloat() * CPARAMETERS::WindowWidth ,RandFloat() * CPARAMETERS::WindowHeight );
	mFitness = 0 ;
	mRotation = RandFloat() * CPARAMETERS::PI * 2 ;
	return ;
}

S2DVector CMineSweeper::GetClosestMine(vector<S2DVector>& objects )
{
	double MinDistance = 99999999 ;
	S2DVector ClosestObject(0 ,0 );
	for(size_t i=0 ;i<objects.size() ;i++ )
	{
		double Length = S2DVector(objects[i] - mPosition ).Length() ;
		if(Length < MinDistance )
		{
			MinDistance = Length ;
			ClosestObject = mPosition - objects[i] ;
			ClosestMine = i ;
		}
	}
	return ClosestObject ;
}

int CMineSweeper::CheckForMine(vector<S2DVector>& mines ,double size )
{
	S2DVector distance = mPosition - mines[ClosestMine] ;
	if(distance.Length() < (size+5))
	{
		return ClosestMine ;
	}
	return -1 ;
}

bool CMineSweeper::Update(vector<S2DVector>& mines )
{
	vector<double> inputs ;
	S2DVector vClosestMine = GetClosestMine(mines );

	vClosestMine.Normalize() ;

	inputs.push_back(vClosestMine.x );
	inputs.push_back(vClosestMine.y );

	inputs.push_back(LookAt.x );
	inputs.push_back(LookAt.y );

	vector<double> outputs = Brain.Update(inputs );

	if(outputs.size() < (size_t)CPARAMETERS::NumOutput )
	{
		return false ;
	}

	LTrack = outputs[0] ;
	RTrack = outputs[1] ;

	double RotForce = LTrack - RTrack ;
	Clamp(RotForce ,-CPARAMETERS::MaxTurnRate ,CPARAMETERS::MaxTurnRate );
	mRotation += RotForce ;
	Speed = (LTrack + RTrack );

	LookAt.x = -sin(mRotation );
	LookAt.y = cos(mRotation );

	mPosition += Speed * LookAt ;
	while(mPosition.x > CPARAMETERS::WindowWidth )mPosition.x -= CPARAMETERS::WindowWidth ;
	while(mPosition.x < 0 )mPosition.x += CPARAMETERS::WindowWidth ;
	while(mPosition.y > CPARAMETERS::WindowHeight )mPosition.y -= CPARAMETERS::WindowHeight ;
	while(mPosition.y < 0 )mPosition.y += CPARAMETERS::WindowHeight ;

	return true ;
}