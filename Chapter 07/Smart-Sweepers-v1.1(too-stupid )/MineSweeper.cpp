#include "MineSweeper.h"
#include "utils.hpp"
#include "CParam.h"

MineSweeper::MineSweeper(void ):m_Rotation(RandFloat()*2*CParam::Pi ),m_LTrack(0.16 ),m_RTrack(0.16 ),
m_ClosestMine(0 )
{
	m_Fitness =CParam::StartEnergy ;
	m_Scale =CParam::SweeperScale ;
	m_Position =S2DVector(RandFloat()*CParam::WindowWidth ,RandFloat()*CParam::WindowHeight );
}

bool MineSweeper::Update(vector<S2DVector>& Mines)
{
	vector<double> inputs ,outputs ;
	S2DVector ClosestMine =GetClosestMine(Mines );

	ClosestMine.Normalize();
	double dot = m_LookAt *ClosestMine;
	int sign = S2DVSign(m_LookAt ,ClosestMine );
	inputs.push_back(dot *sign );

	outputs =m_Brain.Update(inputs );

	if(outputs.size() <CParam::NumOfOutput )
	{
		return false ;
	}

	m_LTrack =outputs[0] ,m_RTrack =outputs[1] ;
	double RotForce =m_LTrack -m_RTrack ;
	Clamp(RotForce ,-CParam::MaxTurnRate ,CParam::MaxTurnRate );

	m_Rotation += RotForce ;
	m_Speed =(m_LTrack +m_RTrack );
	m_LookAt.x = -sin(m_Rotation );
	m_LookAt.y = cos(m_Rotation );
	m_Position += m_LookAt *m_Speed ;

	RectLimit(m_Position ,0 ,0 ,CParam::WindowWidth ,CParam::WindowHeight );

	return true ;
}

void MineSweeper::WorldTransform(vector<SPoint>& sweeper)
{
	C2DMATRIX::TransformSPoints(sweeper ,m_Position.x ,m_Position.y ,m_Scale ,m_Scale ,m_Rotation );
}

static inline double InfDistance(void )
{return CParam::WindowHeight *CParam::WindowHeight +CParam::WindowWidth *CParam::WindowWidth ;}

S2DVector MineSweeper::GetClosestMine(vector<S2DVector>& objects)
{
	double MinDistance =InfDistance();
	S2DVector& closest =objects[0] ;
	for(size_t i=0 ;i<objects.size() ;i++ )
	{
		double len =S2DVLength(m_Position -objects[i]);
		if(len <MinDistance )
		{
			closest =objects[i] ;
			MinDistance =len ;
			m_ClosestMine =i ;
		}
	}
	return closest ;
}

int MineSweeper::CheckCrashToMine(vector<S2DVector>& mines, double size)
{
	S2DVector difference =m_Position -mines[m_ClosestMine] ;
	if(difference.Length() <size +5 )
	{
		return m_ClosestMine ;
	}
	return -1 ;
}

void MineSweeper::Reset(void )
{
	m_Fitness =CParam::StartEnergy ;
	m_ClosestMine =0 ;
	m_Rotation =RandFloat()*2*CParam::Pi ;
	m_Position =S2DVector(RandFloat()*CParam::WindowWidth ,RandFloat()*CParam::WindowHeight );
	return ;
}

