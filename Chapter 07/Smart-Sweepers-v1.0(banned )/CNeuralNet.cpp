#include "CNeuralNet.h"

SNEURON::SNEURON(int NumOfInputs ):NumInputs(NumOfInputs+1 )
{
	for(int i=0 ;i<NumInputs+1 ;i++ )
	{
		weights.push_back(RandClamped());
	}
}

SNEURONLAYER::SNEURONLAYER(int NumOfNeurons ,int NumInputsPerNeuron ):NumNeurons(NumOfNeurons )
{
	for(int i=0 ;i<NumNeurons ;i++ )
	{
		vecNeurons.push_back(SNEURON(NumInputsPerNeuron ));
	}
}

CNeuralNet::CNeuralNet(void )
{
	NumInputs = CPARAMETERS::NumInputs ;
	NumOutputs = CPARAMETERS::NumOutput ;
	NumHiddenLayers = CPARAMETERS::NumHidden ;
	NeuronsPerHiddenLayer = CPARAMETERS::NeuronsPerHiddenLayer ;

	GenerateNet();
}

void CNeuralNet::GenerateNet(void )
{
	if(NumHiddenLayers )
	{
		vecLayers.push_back(SNEURONLAYER(NeuronsPerHiddenLayer ,NumInputs ));
		for(int i=1 ;i<NumHiddenLayers ;i++ )
		{
			vecLayers.push_back(SNEURONLAYER(NeuronsPerHiddenLayer ,NeuronsPerHiddenLayer ));
		}
		vecLayers.push_back(SNEURONLAYER(NumOutputs ,NeuronsPerHiddenLayer ));
	}
	else
	{
		vecLayers.push_back(SNEURONLAYER(NumOutputs ,NumInputs ));
	}
	return ;
}

vector<double> CNeuralNet::GetWeights(void )const 
{
	vector<double > weights ;
	for(int i=0 ;i<NumHiddenLayers+1 ;i++ )
	{
		for(int j=0 ;j<vecLayers[i].NumNeurons ;j++ )
		{
			for(int k=0 ;k<vecLayers[i].vecNeurons[j].NumInputs ;k++ )
			{
				weights.push_back(vecLayers[i].vecNeurons[j].weights[k] );
			}
		}
	}
	return weights ;
}

int CNeuralNet::GetNumOfWeights(void )const
{
	int weights = 0 ;
	for(int i=0 ;i<NumHiddenLayers+1 ;i++ )
	{
		for(int j=0 ;j<vecLayers[i].NumNeurons ;j++ )
		{
			for(int k=0 ;k<vecLayers[i].vecNeurons[j].NumInputs ;k++ )
			{
				weights ++ ;
			}
		}
	}
	return weights ;
}

void CNeuralNet::PutWeights(vector<double>& weights )
{
	int cWeight = 0 ;

	for(int i=0 ;i<NumHiddenLayers+1 ;i++ )
	{
		for(int j=0 ;j<vecLayers[i].NumNeurons ;j++ )
		{
			for(int k=0 ;k<vecLayers[i].vecNeurons[j].NumInputs ;k++ )
			{
				vecLayers[i].vecNeurons[j].weights[k] = weights[cWeight++ ];
			}
		}
	}
	return ;
}

vector<double> CNeuralNet::Update(vector<double>& inputs )
{
	vector<double> outputs ;
	int cWeight = 0 ;
	if(inputs.size() != (size_t )NumInputs )
	{
		return outputs ;
	}
	for(int i=0 ;i<NumHiddenLayers+1 ;i++ )
	{
		if(i )
		{
			inputs = outputs ;
		}
		outputs.clear();

		cWeight = 0 ;
		for(int j=0 ;j<vecLayers[i].NumNeurons ;j++ )
		{
			double netinput = 0 ;
			int NumOfInputs = vecLayers[i].vecNeurons[j].NumInputs ;
			for(int k=0 ;k<NumOfInputs-1 ;k++ )
			{
				netinput += vecLayers[i].vecNeurons[j].weights[k] * inputs[cWeight++];
			}
			netinput += vecLayers[i].vecNeurons[j].weights[NumOfInputs-1] * CPARAMETERS::BIAS ;
			outputs.push_back(Sigmoid(netinput ,CPARAMETERS::ActivationResponse ));

			cWeight = 0 ;
		}
	}
	return outputs ;
}