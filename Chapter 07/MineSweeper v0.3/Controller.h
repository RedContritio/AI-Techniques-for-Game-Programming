#ifndef _CONTROLLER_INC
#define _CONTROLLER_INC
#pragma once

#include "SweeperKiller.h"
#include "MineSweeper.h"
#include "Vector2d.h"
#include "Matrix2d.h"

#include "GeneticAlgorithms.hpp"

#include <vector>
#include <string>
#include <functional>

#include <windows.h>

class Controller
{
private:
	std::vector<SweeperKiller> m_killers;
	std::vector<MineSweeper> m_sweepers;
	std::vector<RedContritio::Vector2d> m_mines;

	int m_numKillers;
	int m_numSweepers;
	int m_numMines;
	
	std::function<void(HDC, const RedContritio::Matrix2d &)> m_SweeperPainter;
	std::function<void(HDC, const RedContritio::Matrix2d &)> m_MinePainter;

	HPEN m_PenRed, m_PenGreen, m_PenBlue, m_PenBlack, m_PenNULL;
	HPEN m_PenOld;

	HBRUSH m_BrushRed, m_BrushBlue, m_BrushYellow, m_BrushNULL;
	HBRUSH m_BrushOld;

	const int &m_cxClient, &m_cyClient;

	int m_ticks, m_generation;

	bool m_isPaused;
	bool m_isFastIterating, m_isSlowIterating;
	bool m_isWatching;

	std::function<MineSweeper(void)> GenerateRandomSweeper;
	std::function<SweeperKiller(void)> GenerateRandomKiller;

	void resetMine(int id);
	void resetMine(void);
public:
	Controller(void);
	~Controller(void);
	void Render(HDC surface);
	bool Update(void);
	bool Pause(void);
	bool Watch(void);
	bool ToggleFastIterating(void);
	bool ToggleSlowIterating(void);
	bool isFastIterating(void) const;
	bool isSlowIterating(void) const;

	void SaveElites(const char *_FileName = "gene.sav") const;
	
};

#endif