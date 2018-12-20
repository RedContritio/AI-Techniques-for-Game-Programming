#ifndef CMINESWEEPER_H
#define CMINESWEEPER_H

//------------------------------------------------------------------------
//
//	Name: CMineSweeper.h
//
//  Author: Mat Buckland 2002
//
//  Desc: Class to create a minesweeper object used in the 'Smart Sweeper'
//        example from the book Game AI Programming with Neural Nets and 
//        Genetic Algorithms.
//
//------------------------------------------------------------------------
#include <vector>
#include <math.h>

#include "CNeuralNet.h"
#include "utils.h"
#include "S2DMatrix.h"
#include "S2DVector.h"
#include "CParam.h"

using namespace std;




class CMinesweeper
{

private:

	CNeuralNet  m_ItsBrain;
  
  //its position in the world
	S2DVector		m_vPosition;

	//direction sweeper is facing
	S2DVector		m_vLookAt;

	//how much it is rotated from its starting position
	double			m_dRotation;

	double			m_dSpeed;

	//to store output from the ANN
	double			m_lTrack, m_rTrack;

	//the sweeper's fitness score. 
	double			m_dFitness;

	//the scale of the sweeper when drawn
	double			m_dScale;

  //index position of closest mine
  int         m_iClosestMine;
  

public:
	

	CMinesweeper();
	
	//updates the ANN with information from the sweepers enviroment
	bool			Update(vector<S2DVector> &mines);

	//used to transform the sweepers vertices prior to rendering
  void			WorldTransform(vector<SPoint> &sweeper);

	//returns a vector to the closest mine
  S2DVector	GetClosestMine(vector<S2DVector> &objects);

  //checks to see if the minesweeper has 'collected' a mine
  int       CheckForMine(vector<S2DVector> &mines, double size);

	void			Reset();
  

	//-------------------accessor functions
	S2DVector	        Position()const{return m_vPosition;}

	void			        IncrementFitness(){++m_dFitness;}

	double		        Fitness()const{return m_dFitness;}

  void              PutWeights(vector<double> &w){m_ItsBrain.PutWeights(w);}

  int               GetNumberOfWeights()const{return m_ItsBrain.GetNumberOfWeights();}

  vector<int>       CalculateSplitPoints()const{return m_ItsBrain.CalculateSplitPoints();}
};


#endif

	
	