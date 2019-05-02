#include "GeneticAlgorithms.hpp"
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

void Mutator::operator()(GENOME &gene) const
{
	for ( unsigned i = 0; i < gene.weights.size(); ++i )
	{
		if ( RandFloat() < Rate )
		{
			gene.weights[i] += RandClamped() * Params::MaxPerturbation;
		}
	}
}

void SplitCrossover::operator()(const GENOME &parent1, const GENOME &parent2, GENOME &child1, GENOME &child2) const
{
	child1 = parent1;
	child2 = parent2;
	if ( RandFloat() < Rate )
	{
		int t1 = RandInt(0, splits.size());
		int t2 = RandInt(0, splits.size());
		int p1 = splits[t1];
		int p2 = splits[t2];

		if ( p1>p2 ) std::swap(p1, p2);

		for ( int i = p1; i < p2; ++i )
		{
			std::swap(child1.weights[i], child1.weights[i]);
		}
	}
}

const GENOME &RouletteSelector::operator()(const std::vector<GENOME> &population, const RelatedFitnessInfo &fitnessInfo) const
{
	double slice = fitnessInfo.TotalFitness * RandFloat();
	for ( unsigned i = 0; i<population.size(); ++i )
	{
		if ( slice <= 0 ) return population[i];
		slice -= population[i].fitness;
	}
	return population[population.size()-1];
}

void EliteKeeper::operator()(const std::vector<GENOME> &prev, std::vector<GENOME> &now) const
{
	for ( int i = 0; i<NumElite; ++i )
	{
		for ( int j = 0; j<NumCopiesPerElite; ++j )
		{
			now.push_back(prev[prev.size()-1-i]);
		}
	}
}

}
}