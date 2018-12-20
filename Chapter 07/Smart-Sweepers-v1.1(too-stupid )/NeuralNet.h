#ifndef NEURALNET__INC
#define NEURALNET__INC
#pragma once
#include <vector>
using std::vector ;

struct Neuron
{
	int m_NumOfInputs ;
	vector<double> m_weights ;
	Neuron(int NumInputs );
};

struct NeuronLayer
{
	int m_NumOfNeurons ;
	vector<Neuron> m_Neurons ;
	NeuronLayer(int NumOfNeurons ,int NumOfPerNeuronInput );
};

class NeuralNet
{
private :
	int m_NumOfInputs ;
	int m_NumOfOutputs ;
	int m_NumOfHiddenLayer ;
	int m_NumOfNeuronsPerHiddenLayer ;
	vector<NeuronLayer> m_Layers ;
	void GenerateNet(void );
	static inline double Sigmoid(double ,double );
public :
	NeuralNet(void );
	vector<double> GetWeights(void )const ;
	void SetWeights(vector<double>& weights );
	vector<double> Update(vector<double> &inputs );
	int GetNumOfWeights(void )const ;
	vector<int> CalculateSplitPoints(void )const ;
};
#endif