#include "Controller.h"
#include "Vector2d.h"
#include "Matrix2d.h"
#include "LogSpawner.h"
#include "Params.h"
#include "utils.hpp"

using RedContritio::Vector2d;
using RedContritio::Matrix2d;
using RedContritio::Utils::RandFloat;

const Vector2d SweeperVertices[] = {Vector2d(4, 0.5), Vector2d(2, 0.5), Vector2d(2, 2), Vector2d(-2, 2),
Vector2d(-2, -2), Vector2d(2, -2), Vector2d(2, -0.5), Vector2d(4, -0.5)};
const Vector2d MineVertices[] = { Vector2d(1, 1), Vector2d(-1, 1), Vector2d(-1, -1), Vector2d(1, -1)};

extern RedContritio::LogSpawner logger;

void LocalSweeperPaint(HDC surface, const Matrix2d &mat);
void LocalMinePaint(HDC surface, const Matrix2d &mat);

auto GetRandomVector2d = [](void) -> Vector2d { return Vector2d(RandFloat()* Params::WindowWidth, RandFloat()* Params::WindowHeight); };

Controller::Controller(void) :
	m_PenBlue(CreatePen(PS_SOLID, 1, RGB(0x00, 0x00, 0xFF))), m_PenGreen(CreatePen(PS_SOLID, 1, RGB(0x00, 0xFF, 0x00))),
	m_PenRed(CreatePen(PS_SOLID, 1, RGB(0xFF, 0x00, 0x00))), m_PenOld(NULL),
	m_numMines(Params::NumMines), m_numSweepers(Params::NumSweepers),
	m_cxClient(Params::WindowWidth), m_cyClient(Params::WindowHeight),
	m_ticks(0),
	GenerateRandomSweeper([](void) -> MineSweeper {
	return MineSweeper(RedContritio::NeuralNetwork::GenerateRandomNeuralNet(Params::NumInputs, Params::NumOutputs,
																			Params::NumHiddenLayers, Params::NeuronsPerHiddenLayer)); }),
	m_SweeperPainter(LocalSweeperPaint), m_MinePainter(LocalMinePaint)
{
	for ( int i = 0; i < Params::NumSweepers; ++i )
	{
		m_sweepers.push_back(GenerateRandomSweeper());
	}
	
	for ( int i = 0; i<Params::NumMines; ++i )
	{
		m_mines.push_back(GetRandomVector2d());
	}
}


Controller::~Controller(void)
{
	auto DeletePen = [](HPEN &pen) { if ( pen ) { DeleteObject(pen); pen = NULL; }};
	
	DeletePen(m_PenBlue);
	DeletePen(m_PenGreen);
	DeletePen(m_PenRed);
}

void Controller::Render(HDC surface)
{
	for ( unsigned i = 0; i<m_sweepers.size(); ++i )
	{
		const MineSweeper &sweeper = m_sweepers[i];
		Matrix2d tmat(sweeper.GetTransformMatrix());

		Ellipse(surface, (int) (sweeper.Position().x - 2), (int) (sweeper.Position().y - 2),
			(int) (sweeper.Position().x + 2), (int) (sweeper.Position().y + 2));
		
		m_SweeperPainter(surface, tmat);
	}

	for ( unsigned i = 0; i<m_mines.size(); ++i )
	{
		Matrix2d tmat(Matrix2d::scale(Params::MineScale) * Matrix2d::move(m_mines[i]));
		m_MinePainter(surface, tmat);
	}
}

bool Controller::Update(void)
{
	if ( m_ticks < Params::NumTicks )
	{
		++m_ticks;
		for ( int i = 0; i<m_numSweepers; ++i )
		{
			bool res = m_sweepers[i].Update(m_mines);
			if ( !res )
			{
				logger.printf("ERROR: Illegal Update, when m_sweeper[%d] is updating.\n", i);
			}
			// TODO: 检查碰撞以及调整适应度
		}
	}
	else
	{
		// TODO: 复位操作
		m_ticks = 0;
	}
	return true;
}



void LocalSweeperPaint(HDC surface, const Matrix2d &mat)
{
	const int NumVertices = sizeof(SweeperVertices)/sizeof(*SweeperVertices);
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
};

void LocalMinePaint(HDC surface, const Matrix2d &mat)
{
	const int NumVertices = sizeof(MineVertices)/sizeof(*MineVertices);
	std::vector<Vector2d> shape;
	for ( unsigned i = 0; i < NumVertices; ++i )
	{
		shape.push_back(MineVertices[i] * mat);
	}

	MoveToEx(surface, (int) (shape[NumVertices-1].x), (int) (shape[NumVertices-1].y), NULL);
	for ( unsigned i = 0; i < NumVertices; ++i )
	{
		LineTo(surface, (int) (shape[i].x), (int) (shape[i].y));
	}
}