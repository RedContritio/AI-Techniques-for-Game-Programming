#ifndef MINESWEEPER__INC
#define MINESWEEPER__INC
#pragma once
#include "NeuralNet.h"
#include "S2DVector.h"
#include "S2DMatrix.h"
#include "CParam.h"

class MineSweeper
{
private :
	NeuralNet m_Brain ;
	S2DVector m_Position ;
	S2DVector m_LookAt ;
	double m_Rotation ;
	double m_Speed ;
	double m_LTrack ,m_RTrack ;
	double m_Fitness ;
	double m_Scale ;
	int m_ClosestMine ;
public :
	MineSweeper(void );
	bool Update(vector<S2DVector> &Mines );
	void WorldTransform(vector<SPoint> &sweeper );
	S2DVector GetClosestMine(vector<S2DVector> &objects );
	int ClosestMineID(void )const {return m_ClosestMine ;}
	int CheckCrashToMine(vector<S2DVector> &mines ,double size );
	void Reset(void );

	S2DVector Position(void )const {return m_Position ;}
	double Angle(void )const {return m_LookAt.Angle() ;}
	void IncrementFitness(void ){m_Fitness++ ;}
	void SetWeights(vector<double> &w ){m_Brain.SetWeights(w );}
	int GetNumOfWeights(void )const {return m_Brain.GetNumOfWeights();}
	vector<int> CalculateSplitPoints(void )const {return m_Brain.CalculateSplitPoints();}
	double Fitness(void )const {return m_Fitness ;}
};

#endif