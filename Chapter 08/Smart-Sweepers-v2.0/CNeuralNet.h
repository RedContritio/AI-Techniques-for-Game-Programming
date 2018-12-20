#ifndef CNERUALNET__INC
#define CNERUALNET__INC
#pragma once

#include <vector>
using std::vector ;

struct NEURON
{
	int m_NumInputs ;
	vector<double> m_weights ;
	NEURON(int NumInputs );
};

struct NeuronLayer
{
	int m_NumNeurons ;
	vector<NEURON> m_Neurons ;
	NeuronLayer(int NumNeurons ,int NumInputsPerNeuron );
};

class CNeuralNet
{
private :
	int m_NumInputs ;
	int m_NumOutputs ;
	int m_NumHiddenLayers ;
	int m_NeuronPerHiddenLayer ;
	vector<NeuronLayer> m_Layers ;
public:
	CNeuralNet(void );
	~CNeuralNet(void );
	void GenerateNet(void );
	vector<double> GetWeights(void )const ;
	int GetNumberOfWeights(void )const ;
	void SetWeights(vector<double>& weights );
	vector<double> Update(vector<double>& inputs );
	vector<int> CalculateSplitPoints(void )const ;
	inline double Sigmoid(double activation ,double response );
};

#endif
