#ifndef __CMINESWEEPER__DEF
#define __CMINESWEEPER__DEF
#pragma once
#include <vector>
#include <cmath>
using std::vector ;

#include "CNeuralNet.h"
#include "S2DMatrix.h"
#include "S2DVector.h"
#include "CParams.h"
#include "utils.hpp"

class CMineSweeper
{
	private :
		CNeuralNet Brain ;
		S2DVector mPosition ;
		S2DVector LookAt ;
		double mRotation ;
		double Speed ;
		double LTrack ,RTrack ;
		double mFitness ;
		double mScale ;
		int ClosestMine ;
	public :
		CMineSweeper(void );
		bool Update(vector<S2DVector>& mines );
		S2DVector GetClosestMine(vector<S2DVector>& objects );
		int CheckForMine(vector<S2DVector>& mines ,double size );
		void Reset(void );
		S2DVector Position(void )const {return mPosition ;}
		void IncrementFitness(void ){if(fabs(LTrack - RTrack)>0.001 )mFitness++ ;}
		double Fitness(void )const {return mFitness ;}
		double Rotation(void )const {return mRotation ;}
		void PutWeights(vector<double> &w ){Brain.PutWeights(w );}
		int GetNumOfWeights(void )const {return Brain.GetNumOfWeights();}
};

#endif
