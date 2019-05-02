#ifndef _SWEEPER_KILLER_INC
#define _SWEEPER_KILLER_INC
#pragma once

#include "NeuralNet.h"
#include "Vector2d.h"

#include "Mine.h"
#include "MineSweeper.h"

#include "windows.h"

class SweeperKiller
{
private:
	RedContritio::NeuralNetwork::NeuralNet m_brain;
	RedContritio::Vector2d m_position;
	RedContritio::Vector2d m_lookat;
	double m_rotation;
	double m_speed;
	double m_lTrack, m_rTrack;
	double m_fitness;
	int m_closestSweeper;
	void UpdateClosestSweeper(const std::vector<MineSweeper>& sweepers);
public:
	SweeperKiller(void);
	SweeperKiller(const RedContritio::NeuralNetwork::NeuralNet &brain);
	~SweeperKiller(void);
	bool Update(const std::vector<MineSweeper> &sweepers);
	void Render(HDC surface) const;
	void Reset(void);

	RedContritio::Vector2d Position(void) const;
	int GetClosestSweeper(void) const;

	int GetNumOfWeights(void) const;
	std::vector<double> GetWeights(void) const;
	void PutWeights(const std::vector<double> &weights);

	std::vector<int> GetSplits(void) const;

	int CheckCollision(const std::vector<MineSweeper> &sweepers) const;

	void IncreaseFitness(void);
	double Fitness(void) const;

	RedContritio::Matrix2d GetTransformMatrix(void) const;

	bool operator<(const SweeperKiller &rhs) const;
	static const int CollisionSize = 4;
	static const int CollisionFailed = -1;
};

#endif