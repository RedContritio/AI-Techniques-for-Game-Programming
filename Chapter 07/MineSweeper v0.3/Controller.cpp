#include "Controller.h"
#include "Vector2d.h"
#include "Matrix2d.h"
#include "LogSpawner.h"
#include "Params.h"
#include "utils.hpp"

#include <algorithm>
#include <vector>

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
	m_pGeneticOperations(NULL),
	m_PenBlue(CreatePen(PS_SOLID, 1, RGB(0x00, 0x00, 0xFF))), m_PenGreen(CreatePen(PS_SOLID, 1, RGB(0x00, 0xFF, 0x00))),
	m_PenRed(CreatePen(PS_SOLID, 1, RGB(0xFF, 0x00, 0x00))), m_PenBlack(CreatePen(PS_SOLID, 1, RGB(0x00, 0x00, 0x00))),
	m_PenNULL(HPEN(GetStockObject(NULL_PEN))),
	m_PenOld(NULL),
	m_BrushBlue(CreateSolidBrush(RGB(0x00, 0x00, 0xFF))), m_BrushRed(CreateSolidBrush(RGB(0xFF, 0x00, 0x00))),
	m_BrushYellow(CreateSolidBrush(RGB(0x00, 0xFF, 0xFF))), m_BrushNULL(HBRUSH(GetStockObject(NULL_BRUSH))),
	m_BrushOld(NULL),
	m_numMines(Params::NumMines), m_numSweepers(Params::NumSweepers),
	m_cxClient(Params::WindowWidth), m_cyClient(Params::WindowHeight),
	m_ticks(0), m_isPaused(false), m_isWatching(false),m_isFastIterating(false), m_isSlowIterating(false),
	GenerateRandomSweeper([](void) -> MineSweeper {
	return MineSweeper(RedContritio::NeuralNetwork::GenerateRandomNeuralNet(Params::NumInputs, Params::NumOutputs,
																			Params::NumHiddenLayers, Params::NeuronsPerHiddenLayer)); }),
	m_SweeperPainter(LocalSweeperPaint), m_MinePainter(LocalMinePaint)
{
	for ( int i = 0; i < Params::NumSweepers; ++i )
	{
		m_sweepers.push_back(GenerateRandomSweeper());
	}
	
	for ( int i = 0; i < Params::NumMines; ++i )
	{
		m_mines.push_back(GetRandomVector2d());
	}

	m_pGeneticOperations = new GeneticPopulation(Params::NumSweepers, m_sweepers[0].GetNumberOfWeights(),
												 Params::MutationRate, Params::CrossoverRate);

//	m_pGeneticOperations->SetSplits(m_sweepers[0].GetSplits());
}


Controller::~Controller(void)
{
	auto DeletePen = [](HPEN &pen) { if ( pen ) { DeleteObject(pen); pen = NULL; }};
	auto DeleteBrush = [](HBRUSH &brush) { if ( brush ) { DeleteObject(brush); brush = NULL; }};
	
	DeletePen(m_PenBlue);
	DeletePen(m_PenGreen);
	DeletePen(m_PenRed);
	DeletePen(m_PenBlack);

	DeleteBrush(m_BrushBlue);
	DeleteBrush(m_BrushRed);
	DeleteBrush(m_BrushYellow);
}

void Controller::Render(HDC surface)
{
	m_PenOld = (HPEN) SelectObject(surface, m_PenNULL);
	m_BrushOld = (HBRUSH) SelectObject(surface, m_BrushNULL);

	SelectObject(surface, m_BrushYellow);
	for ( unsigned i = 0; i<m_mines.size(); ++i )
	{
		Ellipse(surface, (int) (m_mines[i].x - MineSweeper::CollisionSize), (int) (m_mines[i].y - MineSweeper::CollisionSize),
						 (int) (m_mines[i].x + MineSweeper::CollisionSize), (int) (m_mines[i].y + MineSweeper::CollisionSize));
	}

	SelectObject(surface, m_BrushNULL);
	SelectObject(surface, m_PenBlack);
	for ( unsigned i = 0; i<m_mines.size(); ++i )
	{
		Matrix2d tmat(Matrix2d::scale(Params::MineScale) * Matrix2d::move(m_mines[i]));
		m_MinePainter(surface, tmat);
	}

	
	SelectObject(surface, m_PenRed);
	for ( unsigned i = 0; i<m_sweepers.size(); ++i )
	{
		const MineSweeper &sweeper = m_sweepers[i];
		Matrix2d tmat(sweeper.GetTransformMatrix());

		Ellipse(surface, (int) (sweeper.Position().x - 2), (int) (sweeper.Position().y - 2),
			(int) (sweeper.Position().x + 2), (int) (sweeper.Position().y + 2));
		if ( m_isWatching )
		{
			MoveToEx(surface, (int) (sweeper.Position().x), (int) (sweeper.Position().y), NULL);
			LineTo(surface, (int) (m_mines[sweeper.GetClosestMine()].x), (int) (m_mines[sweeper.GetClosestMine()].y));
		}
		
		m_SweeperPainter(surface, tmat);
		if ( i == Params::NumElite * Params::NumCopiesElite ) SelectObject(surface, m_PenOld);
	}

	SelectObject(surface, m_PenOld);
	SelectObject(surface, m_BrushOld);
}

bool Controller::Update(void)
{
	if ( !m_isPaused )
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
				int MineID = m_sweepers[i].CheckCollision(m_mines);
				if ( MineID != MineSweeper::CollisionFailed )
				{
					m_sweepers[i].IncreaseFitness();
					this->resetMine(MineID);
				}
			}
		}
		else
		{
			// TODO: 复位操作
			m_ticks = 0;
			resetMine();
			std::vector<GENOME> generation;
			for ( unsigned i = 0; i<m_sweepers.size(); ++i )
			{
				generation.push_back(GENOME(m_sweepers[i].GetWeights(), m_sweepers[i].Fitness()));
			}

			std::vector<GENOME> next_generation(m_pGeneticOperations->Epoch(generation));

			logger.printf("interating...\n->generation[%02d]: best: %d ,average: %d\n", m_pGeneticOperations->Generation(),
				m_pGeneticOperations->BestFitness(), m_pGeneticOperations->AverageFitness());

			for ( unsigned i = 0; i<m_sweepers.size(); ++i )
			{
				m_sweepers[i].Reset();
				m_sweepers[i].PutWeights(next_generation[i].m_gene);
			}
		}
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

void Controller::resetMine(int id)
{
	m_mines[id] = GetRandomVector2d();
}

void Controller::resetMine(void)
{
	for ( std::vector<Vector2d>::iterator i = m_mines.begin(); i != m_mines.end(); ++i )
	{
		*i = GetRandomVector2d();
	}
}

bool Controller::Pause(void)
{
	return m_isPaused = !m_isPaused;
}

bool Controller::Watch(void)
{
	return m_isWatching = !m_isWatching;
}

bool Controller::ToggleFastIterating(void)
{
	m_isSlowIterating = m_isSlowIterating && m_isFastIterating;
	return m_isFastIterating = !m_isFastIterating;
}

bool Controller::ToggleSlowIterating(void)
{
	m_isFastIterating = m_isSlowIterating && m_isFastIterating;
	return m_isSlowIterating = !m_isSlowIterating;
}

bool Controller::isFastIterating(void) const
{
	return m_isFastIterating;
}

bool Controller::isSlowIterating(void) const
{
	return m_isSlowIterating;
}

void Controller::SaveElites(const char *_FileName) const
{
	if ( MessageBox(NULL, "Are you sure to save elites ?", "Notice", MB_YESNOCANCEL) != IDYES )
	{
		return;
	}
	RedContritio::LogSpawner SaveFile(_FileName);
	SaveFile.SetTimeStamp(false);
	for ( int i = 0; i<Params::NumElite; ++i )
	{
		SaveFile.printf("[ID: %d ]\n", i);
		const MineSweeper &sweeper = m_sweepers[i*Params::NumCopiesElite];
		SaveFile.printf("inputs: %d ,outputs: %d ,hiddenLayers: %d ,NeuronsPerLayer: %d\n",
			Params::NumInputs, Params::NumOutputs, Params::NumHiddenLayers, Params::NeuronsPerHiddenLayer);
		std::vector<double> weights(sweeper.GetWeights());
		for ( unsigned i = 0; i<weights.size(); ++i )
		{
			SaveFile.printf("%lf ", weights[i]);
		}
		SaveFile.printf("\n");
	}
	SaveFile.printf("\n");
}
