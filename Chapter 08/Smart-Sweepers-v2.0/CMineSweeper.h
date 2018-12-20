#ifndef CMineSweeper__INC
#define CMineSweeper__INC
#pragma once
#include <vector>
using std::vector ;
#include <windows.h>

#include "CNeuralNet.h"
#include "S2DVector.h"

class MineSweeper
{
private :
	CNeuralNet m_brain ;
	S2DVector m_position ;
	S2DVector m_lookat ;
	
	double m_rotation ;
	double m_speed ;
	double m_LTrack ,m_RTrack ;

	double m_fitness ;
	double m_scale ;

	double m_SpinBonus ;
	double m_CollisionBonus ;

	vector<SPoint> m_sensors ;
	vector<SPoint> m_transensors ;

	vector<double> m_dSensors ;
	bool m_Collided ;

	void CreateSensors(vector<SPoint>& sensors ,int NumSensors ,double range );
	int CheckForHit(vector<S2DVector>& objects ,double size );
	void CheckSensors(vector<SPoint> objects );
public :
	MineSweeper(void );
	bool Update(vector<SPoint>& objects );
	void WorldTransform(vector<SPoint>& vbs ,double scale );
	void Reset(void );
	void EndRunCalculations(void );
	void RenderStats(HDC surface );
	void RenderPenalties(HDC surface );

	S2DVector Position(void )const {return m_position ;}
	double Rotation(void )const {return m_rotation ;}
	double Fitness(void )const {return m_fitness ;}
	void ZeroEnergy(void ){m_fitness =0 ;}
	double Scale(void )const {return m_scale ;}
	vector<SPoint>& Sensors(void ){return m_transensors ;}
	vector<double>& SensorReadings(void ){return m_dSensors ;}
	bool Collided(void )const {return m_Collided ;}
	void SetWeights(vector<double>& w ){m_brain.SetWeights(w );}
	int GetNumberOfWeights(void )const {return m_brain.GetNumberOfWeights();}
	vector<int> CalculateSplitPoints(void ){return m_brain.CalculateSplitPoints() ;}
};

#endif