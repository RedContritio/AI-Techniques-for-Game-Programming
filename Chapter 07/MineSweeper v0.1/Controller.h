#ifndef _CONTROLLER_INC
#define _CONTROLLER_INC
#pragma once

#include "MineSweeper.h"
#include "Vector2d.h"
#include "Matrix2d.h"

#include <vector>
#include <string>
#include <functional>

#include <windows.h>

class Controller
{
private:
	std::vector<MineSweeper> m_sweepers;
	std::vector<RedContritio::Vector2d> m_mines;

	int m_numSweepers;
	int m_numMines;
	
	std::function<void(HDC, const RedContritio::Matrix2d &)> m_SweeperPainter;
	std::function<void(HDC, const RedContritio::Matrix2d &)> m_MinePainter;

	HPEN m_PenRed, m_PenGreen, m_PenBlue, m_PenOld;

	const int &m_cxClient, &m_cyClient;

	int m_ticks;

	std::function<MineSweeper (void)> GenerateRandomSweeper;
public:
	Controller(void);
	~Controller(void);
	void Render(HDC surface);
	bool Update(void);
};

#endif