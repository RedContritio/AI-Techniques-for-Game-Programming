#include "CNeuralNet.h"
#include "CParam.h"
#include "utils.h"
#include "LogSpawner.h"
using RedContritio::LogSpawner ;
extern LogSpawner loger ;

NEURON::NEURON(int NumInputs ):m_NumInputs(NumInputs )
{
	for(int i=0 ;i<NumInputs ;i++ )
	{
		m_weights.push_back(RandClamped());
	}
}

NeuronLayer::NeuronLayer(int NumNeurons ,int NumInputsPerNeuron ):
m_NumNeurons(NumNeurons )
{
	for(int i=0 ;i<NumNeurons ;i++ )
	{
		m_Neurons.push_back(NEURON(NumInputsPerNeuron ));
	}
}

CNeuralNet::CNeuralNet(void )
{
	m_NumInputs =CPARAM::NumInputs ;
	m_NumOutputs =CPARAM::NumOutputs ;
	m_NumHiddenLayers =CPARAM::NumHidden ;
	m_NeuronPerHiddenLayer =CPARAM::NeuronsPerHiddenLayer ;
	GenerateNet();
}


CNeuralNet::~CNeuralNet(void )
{
	;
}

void CNeuralNet::GenerateNet(void)
{
	if(m_NumHiddenLayers >0 )
	{
		m_Layers.push_back(NeuronLayer(m_NeuronPerHiddenLayer ,m_NumInputs ));
		for(int i=1 ;i<m_NumHiddenLayers ;i++ )
		{
			m_Layers.push_back(NeuronLayer(m_NeuronPerHiddenLayer ,m_NeuronPerHiddenLayer ));
			m_Layers.push_back(NeuronLayer(m_NumOutputs ,m_NeuronPerHiddenLayer ));
		}
		m_Layers.push_back(NeuronLayer(m_NumOutputs ,m_NeuronPerHiddenLayer ));
	}
	else
	{
		m_Layers.push_back(NeuronLayer(m_NumOutputs ,m_NumInputs ));
	}
}

vector<double> CNeuralNet::GetWeights(void) const
{
	vector<double> weights ;
	for(int i=0 ;i<m_NumHiddenLayers+1 ;i++ )
	{
		for(int j=0 ;j<m_Layers[i].m_NumNeurons ;j++ )
		{
			for(int k=0 ;k<m_Layers[i].m_Neurons[j].m_NumInputs ;k++ )
			{
				weights.push_back(m_Layers[i].m_Neurons[j].m_weights[k] );
			}
		}
	}
	return weights ;
}

int CNeuralNet::GetNumberOfWeights(void) const
{
	int nw =0 ;
	for(int i=0 ;i<m_NumHiddenLayers +1 ;i++ )
	{
		for(int j=0 ;j<m_Layers[i].m_NumNeurons ;j++ )
		{
			for(int k=0 ;k<m_Layers[i].m_Neurons[j].m_NumInputs ;k++ )
			{
				nw ++ ;
			}
		}
	}
	return nw ;
}

void CNeuralNet::SetWeights(vector<double>& weights)
{
	int nw =0 ;
	for(int i=0 ;i<m_NumHiddenLayers +1 ;i++ )
	{
		for(int j=0 ;j<m_Layers[i].m_NumNeurons ;j++ )
		{
			for(int k=0 ;k<m_Layers[i].m_Neurons[j].m_NumInputs ;k++ )
			{
				m_Layers[i].m_Neurons[j].m_weights[k] =weights[nw++] ;
			}
		}
	}
	return ;
}

vector<double> CNeuralNet::Update(vector<double>& inputs)
{
	vector<double> outputs ;
	int nw =0 ;
	if(inputs.size() !=m_NumInputs )
	{
		loger.puts("Wrong amount NeuralNet inputs in Update" );
		return outputs ;
	}
	for(int i=0 ;i<m_NumHiddenLayers +1 ;i++ )
	{
		if(i >0 )
		{
			inputs =outputs ;
		}
		outputs.clear();
		nw =0 ;
		for(int j=0 ;j<m_Layers[i].m_NumNeurons ;j++ )
		{
			double netinput =0 ;
			int NumInputs =m_Layers[i].m_Neurons[j].m_NumInputs ;
			for(int k=0 ;k<NumInputs -1 ;k++ )
			{
				netinput += m_Layers[i].m_Neurons[j].m_weights[k] *inputs[nw++ ];
			}
			netinput += m_Layers[i].m_Neurons[j].m_weights[NumInputs-1] *CPARAM::BIAS ;
			outputs.push_back(Sigmoid(netinput ,CPARAM::ActivationResponse ));

			nw =0 ;
		}
	}
	return outputs ;
}

vector<int> CNeuralNet::CalculateSplitPoints(void) const
{
	vector<int> Splits ;
	int nw =0 ;
	for(int i=0 ;i<m_NumHiddenLayers +1 ;i++ )
	{
		for(int j=0 ;j<m_Layers[i].m_NumNeurons ;j++ )
		{
			for(int k=0 ;k<m_Layers[i].m_Neurons[j].m_NumInputs ;k++ )
			{
				nw ++ ;
			}
			Splits.push_back(nw -1 );
		}
	}
	return Splits ;
}

inline double CNeuralNet::Sigmoid(double activation, double response)
{
	return (1.0 /(1.0 +exp(-activation /response )));
}
