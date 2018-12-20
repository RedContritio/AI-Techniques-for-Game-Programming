#include "S2DMatrix.h"
#include "CMineSweeper.h"
#include "CParam.h"
#include "Collision.h"
#include "utils.h"

static const S2DVector OriginPosition =S2DVector(180 ,200 );

MineSweeper::MineSweeper(void):m_rotation(2*RandFloat()*CPARAM::Pi ),m_LTrack(0.16 ),m_RTrack(0.16 ),
m_scale(CPARAM::SweeperScale ),m_Collided(false ),m_SpinBonus(0 ),m_CollisionBonus(0 )
{
	m_position =OriginPosition ; // a initial position
	CreateSensors(m_sensors ,CPARAM::NumSensors ,CPARAM::SensorRange );
}

bool MineSweeper::Update(vector<SPoint>& objects)
{
	vector<double> inputs ;

	CheckSensors(objects );

	for(size_t i=0 ;i<m_dSensors.size() ;i++ )
	{
		inputs.push_back(m_dSensors[i] );
	}

	vector<double> outputs =m_brain.Update(inputs );
	if(outputs.size() <CPARAM::NumOutputs )
	{
		return false ;
	}

	m_LTrack =outputs[0] ;
	m_RTrack =outputs[1] ;

	double RotForce =m_LTrack -m_RTrack ;

	Clamp(RotForce ,-CPARAM::MaxPerturbation ,2*CPARAM::MaxPerturbation );

	m_rotation += RotForce ;

	m_lookat.x =-sin(m_rotation );
	m_lookat.y = cos(m_rotation );

	if(!m_Collided )
	{
		m_speed =(m_LTrack +m_RTrack );
		m_position += m_lookat *m_speed ;
	}

	const double RotationTolerance =0.03 ; //magic number ?
	if(fabs(RotForce ) <RotationTolerance )
	{
		m_SpinBonus += 1 ;
	}

	if(!m_Collided )
	{
		m_CollisionBonus += 1 ;
	}

	return true ;
}

void MineSweeper::WorldTransform(vector<SPoint>& vbs, double scale)
{
	C2DMATRIX::TransformSPoints(vbs ,m_position.x ,m_position.y ,scale ,scale ,m_rotation );
}

void MineSweeper::Reset(void)
{
	m_position =OriginPosition ;
	m_fitness = 0 ;
	m_rotation =2 *RandFloat() *CPARAM::Pi ;
	m_SpinBonus =0 ; 
	m_CollisionBonus =0 ;
}

void MineSweeper::EndRunCalculations(void)
{
	m_fitness +=m_SpinBonus ;
	m_fitness +=m_CollisionBonus ;
}

#pragma warning(push)
#pragma warning(disable:4996)
void MineSweeper::RenderPenalties(HDC surface)
{
	char st[64] ;
	sprintf(st ,"Spin : %g " ,m_SpinBonus );
	TextOut(surface ,200 ,0 ,st ,strlen(st ));

	sprintf(st ,"Collision : %g ",m_CollisionBonus );
	TextOut(surface ,300 ,0 ,st ,strlen(st ));
}
#pragma warning(pop)

void MineSweeper::CreateSensors(vector<SPoint>& sensors, int NumSensors, double range)
{
	sensors.clear();

	double SegmentAngle =CPARAM::Pi /(NumSensors-1 );

	for(int i=0 ;i<NumSensors ;i++ )
	{
		SPoint point ;
		point.x =range * -sin(i *SegmentAngle -0.5*CPARAM::Pi );
		point.y =range *  cos(i *SegmentAngle -0.5*CPARAM::Pi );

		sensors.push_back(point );
	}
}

int MineSweeper::CheckForHit(vector<S2DVector>& objects, double size)
{
	return 0;
}

void MineSweeper::CheckSensors(vector<SPoint> objects)
{
	m_Collided =false ;
	m_transensors =m_sensors ;

	WorldTransform(m_transensors ,1 ); // trans coordinates to world coordinates

	m_dSensors.clear();

	for(size_t i=0 ;i<m_transensors.size() ;i++ )
	{
		bool bHit = false ;
		double dist = 0 ;
		for(size_t seg=0 ;seg<objects.size() ;seg ++ )
		{
			if(LineIntersection2D(m_position ,m_transensors[i] ,objects[seg] ,objects[(seg+1)%objects.size()] ,dist ))
			{
				bHit =true ;
				break ;
			}
		}

		if(bHit )
		{
			if(dist <CPARAM::CollisionDist )
			{
				m_dSensors.push_back(dist );
				m_Collided =true ;
			}
			else
			{
				m_dSensors.push_back(-1 );
			}
		}
	}
}
