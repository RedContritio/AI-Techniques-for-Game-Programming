#include "NeuralNet.h"
#include "LogSpawner.h"
#include "Params.h"
#include "utils.hpp"

extern Params params;
extern RedContritio::LogSpawner logger;

namespace RedContritio
{


namespace NeuralNetwork
{

NeuralNet::NeuralNet(void) : m_NumInputs(0), m_NumOutputs(0), m_NumHiddenLayers(0), m_NumNeuronsPerLayer(0), m_Layers() {}

NeuralNet::NeuralNet(int NumInputs, int NumOutputs, int NumHiddenLayer, int NumNeuronsPerLayer, std::vector<NeuronLayer> Layers) :
	m_NumInputs(NumInputs), m_NumOutputs(NumOutputs), m_NumHiddenLayers(NumHiddenLayer), m_NumNeuronsPerLayer(NumNeuronsPerLayer),
	m_Layers(Layers) {}

NeuralNet::~NeuralNet() {}

std::vector<double> NeuralNet::GetWeights(void) const
{
	std::vector<double> weights;
	for ( int i = 0; i<m_NumHiddenLayers+1; ++i )
	{
		for ( int j = 0; j<m_Layers[i].m_NumNeurons; ++j )
		{
			for ( int k = 0; k<m_Layers[i].m_Neurons[j].m_NumInputs; ++k )
			{
				weights.push_back(this->m_Layers[i].m_Neurons[j].m_weights[k]);
			}
		}
	}
	return weights;
}

int NeuralNet::GetNumberOfWeights(void) const
{
	int NumWeights = 0;
	for ( int i = 0; i<m_NumHiddenLayers+1; ++i )
	{
		for ( int j = 0; j<m_Layers[i].m_NumNeurons; ++j )
		{
			NumWeights += m_Layers[i].m_Neurons[j].m_NumInputs;
		}
	}
	return NumWeights;
}

std::vector<int> NeuralNet::GetSplits(void) const
{
	std::vector<int> splits;
	splits.push_back(0);
	for ( int i = 0; i<m_NumHiddenLayers+1; ++i )
	{
		for ( int j = 0; j<m_Layers[i].m_NumNeurons; ++j )
		{
			splits.push_back(splits[splits.size()-1] + m_Layers[i].m_Neurons[j].m_weights.size());
		}
	}
	return splits;
}

void NeuralNet::PutWeights(const std::vector<double> &weights)
{
	int t = 0;
	for ( int i = 0; i<m_NumHiddenLayers+1; ++i )
	{
		for ( int j = 0; j<m_Layers[i].m_NumNeurons; ++j )
		{
			for ( int k = 0; k<m_Layers[i].m_Neurons[j].m_NumInputs; ++k )
			{
				this->m_Layers[i].m_Neurons[j].m_weights[k] = weights[t];
				++t;
			}
		}
	}
}

std::vector<double> NeuralNet::Update(std::vector<double> &_inputs)
{
	std::vector<double> outputs;
	std::vector<double> inputs;

	if ( _inputs.size() != m_NumInputs )
	{
		return outputs;
	}
	outputs = _inputs;
	for ( int i = 0; i<m_NumHiddenLayers+1; ++i )
	{
		inputs = outputs; // input from previous layer
		outputs.clear();
		for ( int j = 0; j<m_Layers[i].m_NumNeurons; ++j )
		{
			double netinput = 0;
			for ( int k = 0; k<m_Layers[i].m_Neurons[j].m_NumInputs-1; ++k )
			{
//				logger.printf("weights[%d/%d], inputs[%d/%d], m_layers[%d].m_Neurons[%d].m_NumInputs= %d\n",
//					k, m_Layers[i].m_Neurons[j].m_weights.size(), k, inputs.size(), i, j, m_Layers[i].m_Neurons[j].m_NumInputs);
				
					netinput += m_Layers[i].m_Neurons[j].m_weights[k] * inputs[k];
			}

			netinput += m_Layers[i].m_Neurons[j].m_weights[(m_Layers[i].m_Neurons[j].m_NumInputs-1)] * params.Bias;

			outputs.push_back(Utils::Sigmoid(netinput, params.ActivationResponse));
		}
	}
	return outputs;
}

Neuron::Neuron(int NumInputs, std::vector<double> weights) : m_NumInputs(NumInputs+1), m_weights(weights) {}

Neuron GenerateRandomNeuron(int NumInputs)
{
	std::vector<double> weights;
	for ( int i = NumInputs+1; i; --i ) weights.push_back(Utils::RandClamped());
	return Neuron(NumInputs, weights);
}

NeuronLayer::NeuronLayer(int NumNeurons, int NumInputsPerNeuron, std::vector<std::vector<double>> &weights) : m_NumNeurons(NumNeurons)
{
	for ( int i = 0; i<NumInputsPerNeuron; ++i )
	{
		m_Neurons.push_back(Neuron(NumInputsPerNeuron, weights[i]));
	}
}

NeuronLayer::NeuronLayer(int NumNeurons, std::vector<Neuron> &Neurons) : m_NumNeurons(NumNeurons)
{
	for ( int i = 0; i<NumNeurons; ++i )
	{
		m_Neurons = Neurons;
	}
}

NeuronLayer GenerateRandomNeuronLayer(int NumNeurons, int NumInputs)
{
	std::vector<Neuron> Neurons;
	for ( int i = 0; i<NumNeurons; ++i )
	{
		Neurons.push_back(GenerateRandomNeuron(NumInputs));
	}
	return NeuronLayer(NumNeurons, Neurons);
}

NeuralNet GenerateRandomNeuralNet(int NumInputs, int NumOutputs, int NumHiddenLayer, int NumNeuronsPerLayer)
{
	std::vector<NeuronLayer> layers;
	if ( NumHiddenLayer > 0 )
	{
		layers.push_back(GenerateRandomNeuronLayer(NumNeuronsPerLayer, NumInputs));
		for ( int i = 0; i<NumHiddenLayer-1; ++i )
		{
			layers.push_back(GenerateRandomNeuronLayer(NumNeuronsPerLayer, NumNeuronsPerLayer));
		}
		layers.push_back(GenerateRandomNeuronLayer(NumOutputs, NumNeuronsPerLayer));
	}
	else
	{
		layers.push_back(GenerateRandomNeuronLayer(NumOutputs, NumInputs));
	}
	return NeuralNet(NumInputs, NumOutputs, NumHiddenLayer, NumNeuronsPerLayer, layers);
}

}

}