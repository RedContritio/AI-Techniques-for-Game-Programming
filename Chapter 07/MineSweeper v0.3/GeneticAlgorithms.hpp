#ifndef GENETIC_ALGORITHMS_INC
#define GENETIC_ALGORITHMS_INC
#pragma once

#include <vector>
#include <functional>

#include "Params.h"
#include "utils.hpp"

#include <algorithm>

using RedContritio::Utils::RandFloat;
using RedContritio::Utils::RandClamped;
using RedContritio::Utils::RandInt;

namespace RedContritio
{
namespace GeneticAlgorithm
{

struct RelatedFitnessInfo
{
	double BestFitness;
	double TotalFitness;
	double AverageFitness;
	double WorstFitness;
};

template<typename T>
struct _GENOME_base
{
	std::vector<T> weights;
	double fitness;
	_GENOME_base(std::vector<T> _weights = std::vector<T>(), double _fitness = 0.00) :weights(_weights), fitness(_fitness) {}
	bool operator<(const _GENOME_base<T> &rhs) const { return fitness < rhs.fitness;}
};

typedef _GENOME_base<double> GENOME;

// std::function

template<typename T, typename Func1, typename Func2, typename Func3, typename Func4>
void Epoch(const std::vector<_GENOME_base<T> > &prev, std::vector<_GENOME_base<T> > &now, RelatedFitnessInfo *fitnessInfo, Func1 Mutate, Func2 Crossover, Func3 Select, Func4 Initialize)
{
	std::vector<_GENOME_base<T> > genes(prev);

	if ( !genes.size() ) return;

	std::sort(genes.begin(), genes.end());

	if ( fitnessInfo )
	{
		fitnessInfo->WorstFitness = genes[0].fitness;
		fitnessInfo->BestFitness = genes[genes.size()-1].fitness;
		fitnessInfo->TotalFitness = 0;

		for ( unsigned i = 0; i<genes.size(); ++i )
		{
			fitnessInfo->TotalFitness += genes[i].fitness;
		}
		fitnessInfo->AverageFitness = fitnessInfo->TotalFitness / genes.size();
	}

	Initialize(genes, now);

	while ( now.size() < prev.size() )
	{
		const _GENOME_base<T> &parent1 = Select(genes, *fitnessInfo);
		const _GENOME_base<T> &parent2 = Select(genes, *fitnessInfo);

		_GENOME_base<T> child1, child2;

		Crossover(parent1, parent2, child1, child2);

		Mutate(child1);
		Mutate(child2);

		now.push_back(child1);
		now.push_back(child2);
	}
}

struct Mutator
{
	double Rate;
	void operator()(GENOME& gene) const;
	Mutator(double _Rate) : Rate(_Rate) {}
};

struct SplitCrossover
{
	double Rate;
	std::vector<int> splits;
	void operator()(const GENOME &parent1, const GENOME &parent2, GENOME &child1, GENOME &child2) const;
	SplitCrossover(double _Rate, const std::vector<int> &_splits) :Rate(_Rate), splits(_splits) {}
};

struct RouletteSelector
{
	const GENOME& operator()(const std::vector<GENOME>& population, const RelatedFitnessInfo& fitnessInfo) const;
};

struct EliteKeeper
{
	int NumElite;
	int NumCopiesPerElite;
	void operator()(const std::vector<GENOME> &prev, std::vector<GENOME> &now) const;
	EliteKeeper(int _NumElite, int _NumCopies) : NumElite(_NumElite), NumCopiesPerElite(_NumCopies) {}
};

}
}
#endif