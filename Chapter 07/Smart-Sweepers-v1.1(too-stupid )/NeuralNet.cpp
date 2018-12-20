#include "NeuralNet.h"
#include "utils.hpp"
#include "CParam.h"

Neuron::Neuron(int NumInputs ):m_NumOfInputs(NumInputs+1 )
{
	for(int i=0 ;i<m_NumOfInputs ;i++ )m_weights.push_back(RandClamped());
}

NeuronLayer::NeuronLayer(int NumOfNeurons ,int NumOfInputsPerNeuron ):m_NumOfNeurons(NumOfNeurons )
{
	for(int i=0 ;i<m_NumOfNeurons ;i++ )m_Neurons.push_back(Neuron(NumOfInputsPerNeuron ));
}

NeuralNet::NeuralNet(void )
{
	m_NumOfInputs =CParam::NumOfInput ;
	m_NumOfOutputs =CParam::NumOfOutput ;
	m_NumOfHiddenLayer =CParam::NumOfHidden ;
	m_NumOfNeuronsPerHiddenLayer =CParam::NeuronsPerHiddenLayer ;
	GenerateNet();
}

void NeuralNet::GenerateNet(void )
{
	// Set Input Layer as a default layer ,so it's not necessary to generate it 
	if(m_NumOfHiddenLayer >0 )
	{
		m_Layers.push_back(NeuronLayer(m_NumOfNeuronsPerHiddenLayer ,m_NumOfInputs ));
		for(int i=0 ;i<m_NumOfHiddenLayer-1 ;i++ )
		{
			m_Layers.push_back(NeuronLayer(m_NumOfNeuronsPerHiddenLayer ,m_NumOfNeuronsPerHiddenLayer ));
		}
		m_Layers.push_back(NeuronLayer(m_NumOfOutputs ,m_NumOfNeuronsPerHiddenLayer ));
	}
	else
	{
		m_Layers.push_back(NeuronLayer(m_NumOfOutputs ,m_NumOfInputs ));
	}
}

inline double NeuralNet::Sigmoid(double netinput , double response )
{
	return 1.0/(1.0 + exp(-netinput /response ));
}

vector<double> NeuralNet::GetWeights(void )const 
{
	vector<double> weights ;
	for(int i=0 ;i<m_NumOfNeuronsPerHiddenLayer+1 ;i++ )
	{
		for(int j=0 ;j<m_Layers[i].m_NumOfNeurons ;i++ )
		{
			for(int k=0 ;k<m_Layers[i].m_Neurons[j].m_NumOfInputs ;k++ )
			{
				weights.push_back(m_Layers[i].m_Neurons[j].m_weights[k] );
			}
		}
	}
	return weights ;
}

void NeuralNet::SetWeights(vector<double>& weights )
{
	int pW =0 ;
	for(int i=0 ;i<m_NumOfHiddenLayer+1 ;i++ )
	{
		for(int j=0 ;j<m_Layers[i].m_NumOfNeurons ;j++ )
		{
			for(int k=0 ;k<m_Layers[i].m_Neurons[j].m_NumOfInputs ;k++ )
			{
				m_Layers[i].m_Neurons[j].m_weights[k] =weights[pW++] ;
			}
		}
	}
	return ;
}

vector<double> NeuralNet::Update(vector<double>& inputs)
{
	vector<double> outputs ;
	int pInput =0 ;
	if(inputs.size() != m_NumOfInputs )return outputs ;

	for(int i=0 ;i<m_NumOfHiddenLayer+1 ;i++ )
	{
		if(i >0 ) // Not the 1st hidden layer
		{
			inputs =outputs ;
		}

		outputs.clear();
		pInput =0 ;

		for(int j=0 ;j<m_Layers[i].m_NumOfNeurons ;j++ )
		{
			double netinput =0 ;
			int NumOfInputs =m_Layers[i].m_Neurons[j].m_NumOfInputs ;
			for(int k=0 ;k<NumOfInputs -1 ;k++ )
			{
				netinput += m_Layers[i].m_Neurons[j].m_weights[k] *inputs[pInput++];
			}
			netinput +=m_Layers[i].m_Neurons[j].m_weights[NumOfInputs -1] *CParam::BIAS ;
			outputs.push_back(Sigmoid(netinput ,CParam::ActivationResponce ));
			pInput =0 ;
		}
	}

	return outputs ;
}

int NeuralNet::GetNumOfWeights(void) const
{
	int NumOfWeights =0 ;
	for(int i=0 ;i<m_NumOfHiddenLayer+1 ;i++ )
	{
		for(int j=0 ;j<m_Layers[i].m_NumOfNeurons ;j++ )
		{
			for(int k=0 ;k<m_Layers[i].m_Neurons[j].m_NumOfInputs ;k++ )
			{
				NumOfWeights ++ ;
			}
		}
	}
	return NumOfWeights ;
}

vector<int> NeuralNet::CalculateSplitPoints(void) const
{
	vector<int> SplitPoints ;
	int pW =0 ;
	for(int i=0 ;i<m_NumOfHiddenLayer+1 ;i++ )
	{
		for(int j=0 ;j<m_Layers[i].m_NumOfNeurons ;j++ )
		{
			for(int k=0 ;k<m_Layers[i].m_Neurons[j].m_NumOfInputs ;k++ )
			{
				pW++ ;
			}
			SplitPoints.push_back(pW-1 );
		}
	}
	return SplitPoints ;
}

