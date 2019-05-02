#include "SweeperKiller.h"
#include "MineSweeper.h"
#include "Vector2d.h"
#include "Matrix2d.h"
#include "Params.h"
#include "utils.hpp"

#include "LogSpawner.h"

#include <functional>
#include <algorithm>
#include <cassert>
#include <cmath>

using RedContritio::Utils::RandFloat;
using RedContritio::Utils::Clamp;

using RedContritio::Vector2d;
using RedContritio::Matrix2d;

extern RedContritio::LogSpawner logger;

const Vector2d KillerVertices[] = { Vector2d(1, 0), Vector2d(-1, 1), Vector2d(0, 2), Vector2d(-2, 1), Vector2d(-3, 2), Vector2d(-3, 1), 
Vector2d(-4, 0), Vector2d(-3, -1), Vector2d(-3, -2), Vector2d(-2, -1), Vector2d(0, -2), Vector2d(-1, -1)};

inline void CirculantWarp(double &value, double left, double right)
{
	double length(right - left);
	while ( value > right ) value -= length;
	while ( value < left ) value += length;
}

void SweeperKiller::UpdateClosestSweeper(const std::vector<MineSweeper> &sweepers)
{
	m_closestSweeper = 0;
	Vector2d world(Params::WindowWidth, Params::WindowHeight);
	double dis = 1.0 * world.length();
	for ( unsigned i = 0; i<sweepers.size(); ++i )
	{
		Vector2d vec(VectorDifferenceInWorld(m_position, sweepers[i].m_position, world));
		if ( vec.length() < dis )
		{
			m_closestSweeper = i;
			dis = vec.length();
		}
	}
	return;
}

SweeperKiller::SweeperKiller(void) : m_brain(), m_position(), m_lookat(), m_rotation(),
	m_speed(), m_lTrack(), m_rTrack(), m_fitness(), m_closestSweeper()
{
	m_position = GetRandomVector2d();
}

SweeperKiller::SweeperKiller(const RedContritio::NeuralNetwork::NeuralNet &brain) : m_brain(brain), m_position(), m_lookat(), m_rotation(),
m_speed(), m_lTrack(), m_rTrack(), m_fitness(), m_closestSweeper()
{
	m_position = GetRandomVector2d();
}


SweeperKiller::~SweeperKiller(void)
{
}

bool SweeperKiller::Update(const std::vector<MineSweeper> &sweepers)
{
	UpdateClosestSweeper(sweepers);

	std::vector<double> inputs;
	Vector2d targetDirection(Normalize(sweepers[m_closestSweeper].m_position - m_position));

	inputs.push_back(targetDirection.x);
	inputs.push_back(targetDirection.y);

	double c = fabs(targetDirection * m_lookat);
	double angle = acos(c) * sign(m_lookat, targetDirection);
	inputs.push_back(angle);

	inputs.push_back(sweepers[m_closestSweeper].m_rotation);
	inputs.push_back(sweepers[m_closestSweeper].m_speed);

	std::vector<double> outputs(m_brain.Update(inputs));
	if ( outputs.size() < (unsigned) (Params::NumKillerOutputs) )
	{
		return false;
	}

	m_lTrack = outputs[0];
	m_rTrack = outputs[1];

	double RotForce = Clamp(m_rTrack - m_lTrack, -Params::MaxTurnRate, Params::MaxTurnRate);
	m_rotation += RotForce;

	m_speed = m_lTrack + m_rTrack;
	m_lookat = Vector2d(cos(m_rotation), sin(m_rotation));

	m_position += m_lookat * m_speed;

	CirculantWarp(m_position.x, 0, Params::WindowWidth);
	CirculantWarp(m_position.y, 0, Params::WindowHeight);

	return true;
}

void SweeperKiller::Render(HDC surface) const
{
	static const int NumVertices = sizeof(KillerVertices)/sizeof(*KillerVertices);
	Ellipse(surface, (int) (m_position.x - 2), (int) (m_position.y - 2),
		(int) (m_position.x + 2), (int) (m_position.y + 2));
	Matrix2d mat(GetTransformMatrix());
	std::vector<Vector2d> shape;
	for ( unsigned i = 0; i < NumVertices; ++i )
	{
		shape.push_back(KillerVertices[i] * mat);
	}

	MoveToEx(surface, (int) (shape[NumVertices-1].x), (int) (shape[NumVertices-1].y), NULL);
	for ( unsigned i = 0; i < NumVertices; ++i )
	{
		LineTo(surface, (int) (shape[i].x), (int) (shape[i].y));
	}
}

void SweeperKiller::Reset(void)
{
	m_position = GetRandomVector2d();
	m_fitness = 0;
	m_rotation = RandFloat(0, 2*Params::PI);
}

int SweeperKiller::GetNumOfWeights(void) const
{
	return m_brain.GetNumberOfWeights();
}

std::vector<double> SweeperKiller::GetWeights(void) const
{
	return m_brain.GetWeights();
}

void SweeperKiller::PutWeights(const std::vector<double> &weights)
{
	m_brain.PutWeights(weights);
}

std::vector<int> SweeperKiller::GetSplits(void) const
{
	return m_brain.GetSplits();
}

int SweeperKiller::CheckCollision(const std::vector<MineSweeper> &sweepers) const
{
	if ( Vector2dLength(VectorDifferenceInWorld(m_position, sweepers[m_closestSweeper].Position(),
		Vector2d(Params::WindowWidth, Params::WindowHeight))) <= CollisionSize )
	{
		return m_closestSweeper;
	}
	else
	{
		return CollisionFailed;
	}
}

void SweeperKiller::IncreaseFitness(void)
{
	++m_fitness;
}

double SweeperKiller::Fitness(void) const
{
	return m_fitness;
}

RedContritio::Matrix2d SweeperKiller::GetTransformMatrix(void) const
{
	return Matrix2d::transform(m_position.x, m_position.y, Params::KillerScale, Params::KillerScale, m_rotation);
}

bool SweeperKiller::operator<(const SweeperKiller &rhs) const
{
	return m_fitness < rhs.m_fitness;
}

Vector2d SweeperKiller::Position(void) const
{
	return m_position;
}

int SweeperKiller::GetClosestSweeper(void) const
{
	return m_closestSweeper;
}