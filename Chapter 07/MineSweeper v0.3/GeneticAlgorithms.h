#ifndef GENETIC_ALGORITHMS_INC
#define GENETIC_ALGORITHMS_INC
#pragma once

#include <vector>

struct GENOME
{
	std::vector<double> m_gene;
	double m_fitness;
	GENOME(std::vector<double> weights = std::vector<double>(), double fitness = 0.00);
	bool operator<(const GENOME &rhs) const;
};

class GeneticPopulation
{
private:
	std::vector<GENOME> m_genes;
	int m_size;
	int m_ChromoLength;

	double m_totalFitness;
	double m_bestFitness;
	double m_averageFitness;
	double m_worstFitness;

	int m_fittestGenome;

	double m_mutationRate;
	double m_crossoverRate;

	int m_generation;

	std::vector<int> m_splits;

	void Crossover(const std::vector<double> &parent1, const std::vector<double> &parent2,
					std::vector<double> &child1, std::vector<double> &child2);

	void Mutate(std::vector<double> &genome);
	const GENOME &GetChromoRoulette(void) const;
	void GrabNBest(const std::vector<GENOME> &prev, std::vector<GENOME> &population,int _NumElite, int _NumCopiesElite);
	void UpdateRelatedFitness(void);
public:
	GeneticPopulation(int _size, int _numWeights, double _mutatteRate, double _crossoverRate);
	~GeneticPopulation(void);
	std::vector<GENOME> Epoch(const std::vector<GENOME> &prev_pop); // push GENOME and return next generation
	std::vector<GENOME> GetChromos(void) const;
	double AverageFitness(void) const;
	double BestFitness(void) const;
	int Generation(void) const;
	void SetSplits(const std::vector<int> &spilts);
};

#endif