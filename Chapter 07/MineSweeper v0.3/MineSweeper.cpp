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

extern Params params;

const double MaxTrackSpeed = 0.16;

using RedContritio::Utils::RandFloat;
using RedContritio::Utils::Clamp;

using RedContritio::Vector2d;
using RedContritio::Matrix2d;

extern RedContritio::LogSpawner logger;

const Vector2d SweeperVertices[] = { Vector2d(4, 0.5), Vector2d(2, 0.5), Vector2d(2, 2), Vector2d(-2, 2),
Vector2d(-2, -2), Vector2d(2, -2), Vector2d(2, -0.5), Vector2d(4, -0.5) };

inline void CirculantWarp(double &value, double left, double right)
{
	double length(right - left);
	while(value > right) value -= length;
	while ( value < left ) value += length;
}

MineSweeper::MineSweeper(void) :m_brain(),
m_position(), m_lookat(), m_rotation(), m_speed(), m_lTrack(MaxTrackSpeed), m_rTrack(MaxTrackSpeed),
m_fitness(), m_closestMine(0)
{
	m_position = GetRandomVector2d();
}

MineSweeper::MineSweeper(const RedContritio::NeuralNetwork::NeuralNet &brain) : m_brain(brain),
m_position(), m_lookat(), m_rotation(), m_speed(), m_lTrack(MaxTrackSpeed), m_rTrack(MaxTrackSpeed),
m_fitness(), m_closestMine(0)
{
	m_position = GetRandomVector2d();
}


MineSweeper::~MineSweeper()
{
}

bool MineSweeper::Update(const std::vector<Vector2d> &mines)
{
	UpdateClosestMine(mines);

	std::vector<double> inputs;
	Vector2d targetDirection(Normalize(mines[m_closestMine] - m_position));

	inputs.push_back(targetDirection.x);
	inputs.push_back(targetDirection.y);
	
	double c = fabs( targetDirection * m_lookat );
	double angle = acos(c) * sign(m_lookat, targetDirection);
	inputs.push_back(angle);

	std::vector<double> outputs(m_brain.Update(inputs));
	if ( outputs.size() < (unsigned)(Params::NumSweeperOutputs) )
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


void MineSweeper::Render(HDC surface) const
{
	static const int NumVertices = sizeof(SweeperVertices)/sizeof(*SweeperVertices);
	Ellipse(surface, (int) (m_position.x - 2), (int) (m_position.y - 2),
		(int) (m_position.x + 2), (int) (m_position.y + 2));

	Matrix2d mat(GetTransformMatrix());
	std::vector<Vector2d> shape;
	for ( unsigned i = 0; i < NumVertices; ++i )
	{
		shape.push_back(SweeperVertices[i] * mat);
	}

	MoveToEx(surface, (int) (shape[NumVertices-1].x), (int) (shape[NumVertices-1].y), NULL);
	for ( unsigned i = 0; i < NumVertices; ++i )
	{
		LineTo(surface, (int) (shape[i].x), (int) (shape[i].y));
	}
}

void MineSweeper::Reset(void)
{
	m_position = GetRandomVector2d();
	m_fitness = 0;
	m_rotation = RandFloat(0, 2*Params::PI);
	return;
}

Vector2d MineSweeper::Position(void) const
{
	return m_position;
}

void MineSweeper::IncreaseFitness(void)
{
	++m_fitness;
}

double MineSweeper::Fitness(void) const
{
	return m_fitness;
}

RedContritio::Matrix2d MineSweeper::GetTransformMatrix(void) const
{
	return Matrix2d::transform(m_position.x, m_position.y, Params::SweeperScale, Params::SweeperScale, m_rotation);
}

std::vector<double> MineSweeper::GetWeights(void) const
{
	return m_brain.GetWeights();
}

void MineSweeper::PutWeights(std::vector<double> &w)
{
	m_brain.PutWeights(w);
}

int MineSweeper::GetNumberOfWeights(void) const
{
	return m_brain.GetNumberOfWeights();
}

std::vector<int> MineSweeper::GetSplits(void) const
{
	return m_brain.GetSplits();
}

int MineSweeper::GetClosestMine(void) const
{
	return m_closestMine;
}

int MineSweeper::CheckCollision(const std::vector<RedContritio::Vector2d> &mines) const
{
	if ( Vector2dLength(VectorDifferenceInWorld(m_position, mines[m_closestMine],
												Vector2d(Params::WindowWidth, Params::WindowHeight))) <= CollisionSize )
	{
		return m_closestMine;
	}
	else
	{
		return CollisionFailed;
	}
}

bool MineSweeper::operator<(const MineSweeper &rhs) const
{
	return this->m_fitness < rhs.m_fitness;
}


void MineSweeper::UpdateClosestMine(const std::vector<Vector2d> &mines)
{
	m_closestMine = 0;
	Vector2d world(Params::WindowWidth, Params::WindowHeight);
	double dis = 1.0 * world.length();
	for ( unsigned i = 0; i<mines.size(); ++i )
	{
		Vector2d vec(VectorDifferenceInWorld(m_position, mines[i], world));
		if ( vec.length() < dis )
		{
			m_closestMine = i;
			dis = vec.length();
		}
	}
	return;
}

Vector2d VectorDifferenceInWorld(Vector2d v1, Vector2d v2, Vector2d worldsize)
{
	double dx0 = v2.x - v1.x + worldsize.x;
	double dx1 = v2.x - v1.x;
	double dx2 = v2.x - v1.x - worldsize.x;
	double dy0 = v2.y - v1.y + worldsize.y;
	double dy1 = v2.y - v1.y;
	double dy2 = v2.y - v1.y - worldsize.y;
	double dx = fabs(dx0) < fabs(dx1) ? dx0 : dx1;
	dx = fabs(dx) < fabs(dx2) ? dx : dx2;
	double dy = fabs(dy0) < fabs(dy1) ? dy0 : dy1;
	dy = fabs(dy) < fabs(dy2) ? dy : dy2;
	return Vector2d(dx, dy);
}
