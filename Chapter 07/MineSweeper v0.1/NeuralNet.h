#ifndef NEURALNET_INC
#define NEURALNET_INC
#pragma once

#include <vector>

namespace RedContritio
{

namespace NeuralNetwork
{

/* Neuron Operations */
struct Neuron
{
	int m_NumInputs;
	std::vector<double> m_weights;
	Neuron(int NumInputs, std::vector<double> weights = std::vector<double>());
	Neuron(const Neuron &pro) = default;
};

Neuron GenerateRandomNeuron(int NumInputs);

/* Neuron Layer Operations */
struct NeuronLayer
{
	int m_NumNeurons;
	std::vector<Neuron> m_Neurons;
	NeuronLayer(int NumNeurons, int NumInputsPerNeuron, std::vector<std::vector<double>> &weights );
	NeuronLayer(int NumNeurons, std::vector<Neuron> &Neurons);
	NeuronLayer(const NeuronLayer &pro) = default;
};

NeuronLayer GenerateRandomNeuronLayer(int NumNeurons, int NumInputs);

/* Neural Net Operations */
class NeuralNet
{
private:
	int m_NumInputs;
	int m_NumOutputs;
	int m_NumHiddenLayers;
	int m_NumNeuronsPerLayer;
	std::vector<NeuronLayer> m_Layers;
public:
	NeuralNet(void) = default;
	NeuralNet(int NumInputs, int NumOutputs, int NumHiddenLayer, int NumNeuronsPerLayer, std::vector<NeuronLayer> Layers);
	~NeuralNet(void);
	NeuralNet(const NeuralNet &pro) = default;
	std::vector<double> GetWeights(void) const;
	int GetNumberOfWeights(void) const;
	void PutWeights(const std::vector<double> &weights);
	std::vector<double> Update(std::vector<double> &inputs);
	NeuralNet &operator=(const NeuralNet &rhs) = default;
};

NeuralNet GenerateRandomNeuralNet(int NumInputs, int NumOutputs, int NumHiddenLayer, int NumNeuronsPerLayer);

}

}
#endif