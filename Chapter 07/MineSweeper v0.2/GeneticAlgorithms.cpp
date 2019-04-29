#include "GeneticAlgorithms.h"
#include "Params.h"
#include "utils.hpp"

#include <algorithm>

using RedContritio::Utils::RandFloat;
using RedContritio::Utils::RandClamped;
using RedContritio::Utils::RandInt;

GENOME::GENOME(std::vector<double> weights, double fitness) :m_gene(weights), m_fitness(fitness)
{
}

bool GENOME::operator<(const GENOME &rhs) const
{
	return m_fitness < rhs.m_fitness;
}

void GeneticPopulation::Crossover(const std::vector<double> &parent1, const std::vector<double> &parent2, std::vector<double> &child1, std::vector<double> &child2)
{
	child1 = parent1;
	child2 = parent2;
	if ( RandFloat() < m_crossoverRate )
	{
		int p1 = RandInt(0, parent1.size());
		int p2 = RandInt(0, parent1.size());
		
		if ( m_splits.size() )
		{
			int t1 = RandInt(0, m_splits.size());
			int t2 = RandInt(0, m_splits.size());
			p1 = m_splits[t1];
			p2 = m_splits[t2];
		}

		if ( p1>p2 ) std::swap(p1, p2);

		for ( int i = p1; i<p2; ++i )
		{
			std::swap(child1[i], child2[i]);
		}
	}
}

void GeneticPopulation::Mutate(std::vector<double> &genome)
{
	for ( unsigned i = 0; i < genome.size(); ++i )
	{
		if ( RandFloat() < m_mutationRate )
		{
			genome[i] += RandClamped() * Params::MaxPerturbation;
		}
	}
}

const GENOME &GeneticPopulation::GetChromoRoulette(void) const
{
	double slice = m_totalFitness * RandFloat();
	double tmp = 0;
	for ( unsigned i = 0; i<m_genes.size(); ++i )
	{
		if ( tmp >= slice ) return m_genes[i];
		tmp += m_genes[i].m_fitness;
	}
	return m_genes[m_genes.size()-1];
}

void GeneticPopulation::GrabNBest(const std::vector<GENOME> &prev, std::vector<GENOME> &population, int _NumElite, int _NumCopiesElite)
{
	for ( ; _NumElite; --_NumElite )
	{
		for ( int i = _NumCopiesElite; i>0; --i )
		{
			population.push_back(prev[(m_size-1)-_NumElite]);
		}
	}
}

void GeneticPopulation::UpdateRelatedFitness(void)
{
	m_totalFitness = 0;
	m_bestFitness = m_genes[0].m_fitness;
	m_worstFitness = m_genes[0].m_fitness;

	for ( unsigned i = 0; i<m_genes.size(); ++i )
	{
		m_totalFitness += m_genes[i].m_fitness;
		if ( m_bestFitness < m_genes[i].m_fitness ) m_bestFitness = m_genes[i].m_fitness;
		if ( m_worstFitness > m_genes[i].m_fitness ) m_worstFitness = m_genes[i].m_fitness;
	}
	m_averageFitness = m_totalFitness / m_genes.size();
}

GeneticPopulation::GeneticPopulation(int _size, int _numWeights, double _mutatteRate, double _crossoverRate) :
	m_genes(std::vector<GENOME>()), m_size(_size), m_ChromoLength(_numWeights),
	m_totalFitness(0), m_bestFitness(0), m_averageFitness(0), m_worstFitness(0),
	m_fittestGenome(0),
	m_mutationRate(_mutatteRate), m_crossoverRate(_crossoverRate),
	m_generation(0), m_splits()
{
	for ( int i = 0; i<m_size; ++i )
	{
		m_genes.push_back(GENOME());
		for ( int j = 0; j<m_ChromoLength; ++j )
		{
			m_genes[i].m_gene.push_back(RandClamped());
		}
	}
}

GeneticPopulation::~GeneticPopulation(void)
{
	// do nothing;
}

std::vector<GENOME> GeneticPopulation::Epoch(const std::vector<GENOME> &prev_pop)
{
	m_genes = prev_pop;

	std::sort(m_genes.begin(), m_genes.end());

	UpdateRelatedFitness();

	std::vector<GENOME> NewGeneration;

	if ( !(Params::NumCopiesElite * Params::NumElite % 2) )
	{
		GrabNBest(m_genes, NewGeneration, Params::NumElite, Params::NumCopiesElite);
	}

	while ( (int)(NewGeneration.size()) < m_size )
	{
		const GENOME &parent1 = GetChromoRoulette();
		const GENOME &parent2 = GetChromoRoulette();

		std::vector<double> child1, child2;

		Crossover(parent1.m_gene, parent2.m_gene, child1, child2);

		Mutate(child1);
		Mutate(child2);

		NewGeneration.push_back(GENOME(child1, 0));
		NewGeneration.push_back(GENOME(child2, 0));
	}

	m_genes = NewGeneration;
	++m_generation;
	return m_genes;
}

std::vector<GENOME> GeneticPopulation::GetChromos(void) const
{
	return m_genes;
}

double GeneticPopulation::AverageFitness(void) const
{
	return m_averageFitness;
}

double GeneticPopulation::BestFitness(void) const
{
	return m_bestFitness;
}

int GeneticPopulation::Generation(void) const
{
	return m_generation;
}

void GeneticPopulation::SetSplits(const std::vector<int> &splits)
{
	m_splits = splits;
}
