#ifndef MINESWEEPER_INC
#define MINESWEEPER_INC
#pragma once

#include "NeuralNet.h"
#include "Vector2d.h"
#include "Matrix2d.h"

#include <vector>

class MineSweeper
{
private:
	RedContritio::NeuralNetwork::NeuralNet m_brain;
	RedContritio::Vector2d m_position;
	RedContritio::Vector2d m_lookat;
	double m_rotation;
	double m_speed;
	double m_lTrack, m_rTrack;
	double m_fitness;
	int m_closestMine;
	void UpdateClosestMine(const std::vector<RedContritio::Vector2d> &mines);
public:
	MineSweeper(void);
	MineSweeper(const RedContritio::NeuralNetwork::NeuralNet &brain);
	~MineSweeper(void);
	bool Update(std::vector<RedContritio::Vector2d> &mines);
	void Reset(void);

	void IncreaseFitness(void);
	RedContritio::Vector2d Position(void) const;
	double Fitness(void) const;

	void PutWeights(std::vector<double> &w);
	int GetNumberOfWeights(void) const;

	RedContritio::Matrix2d GetTransformMatrix(void) const;
};

#endif
