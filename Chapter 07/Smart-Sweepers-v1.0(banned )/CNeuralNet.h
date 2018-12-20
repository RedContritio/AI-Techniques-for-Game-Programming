#ifndef __CNEURALNET__DEF
#define __CNEURALNET__DEF
#pragma once
#include <cstdio>
#include <cmath>

#include "utils.hpp"
#include "CParams.h"

#include <vector>
using std::vector ;

struct SNEURON
{
	int NumInputs ;
	vector<double> weights ;

	SNEURON(int );
};

struct SNEURONLAYER
{
	int NumNeurons ;
	vector<SNEURON> vecNeurons ;
	SNEURONLAYER(int ,int );
};

class CNeuralNet
{
	private:
		int NumInputs ;
		int NumOutputs ;
		int NumHiddenLayers ;
		int NeuronsPerHiddenLayer ;

		vector<SNEURONLAYER> vecLayers ;
	public :
		CNeuralNet(void );
		void GenerateNet(void );
		vector<double> GetWeights(void )const ;
		int GetNumOfWeights(void )const ;
		void PutWeights(vector<double>& weights );
		vector<double> Update(vector<double>& inputs );

		inline double Sigmoid(double netinput ,double response )
		{
			return 1/(1+exp(-netinput / response));
		}
};
#endif