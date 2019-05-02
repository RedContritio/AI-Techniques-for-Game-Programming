#ifndef MINESWEEPER_INC
#define MINESWEEPER_INC
#pragma once

#include "NeuralNet.h"
#include "Vector2d.h"
#include "Matrix2d.h"

#include <vector>

#include <windows.h>

const int MINESWEEPER_COLLISION_FAILED = -1;

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
	bool Update(const std::vector<RedContritio::Vector2d> &mines);
	void Render(HDC surface) const;
	void Reset(void);

	void IncreaseFitness(void);
	RedContritio::Vector2d Position(void) const;
	double Fitness(void) const;

	std::vector<double> GetWeights(void) const;
	void PutWeights(std::vector<double> &w);
	int GetNumberOfWeights(void) const;

	std::vector<int> GetSplits(void) const;

	int GetClosestMine(void) const;

	int CheckCollision(const std::vector<RedContritio::Vector2d> &mines) const;

	bool operator<(const MineSweeper &rhs) const;

	RedContritio::Matrix2d GetTransformMatrix(void) const;
	static const int CollisionSize = 2;
	static const int CollisionFailed = -1;

	friend class SweeperKiller;
};

RedContritio::Vector2d VectorDifferenceInWorld(RedContritio::Vector2d v1, RedContritio::Vector2d v2, RedContritio::Vector2d worldsize);
#endif
