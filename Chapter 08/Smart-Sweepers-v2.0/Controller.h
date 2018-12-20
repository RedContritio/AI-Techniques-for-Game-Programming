#ifndef CONTROLLER__INC
#define CONTROLLER__INC
#pragma once
#include <vector>
using std::vector ;
#include <queue>
using std::queue ;

#include "utils.h"
#include "S2DMatrix.h"
#include "S2DVector.h"
#include "CGenAlg.h"
#include "CMineSweeper.h"

#include <cstdio>
#include <windows.h>

class Controller
{
private :
	queue<int> m_keys ;
	int ProcessKeys(void );
protected :
	vector<GENOME> m_population ;
	vector<MineSweeper> m_sweepers ;
	CGENALG *m_pGA ;
	int m_NumSweepers ;
	int m_NumWeightsInNN ;
	vector<SPoint> m_SweeperVB ;
	vector<SPoint> m_ObjectsVB ;
	vector<double> m_AverageFitness ;
	vector<double> m_BestFitness ;

	HPEN m_PenRed ,m_PenGreen ,m_PenBlue ,m_PenOld ;
	HPEN m_PenGrayDot ,m_PenRedDot ;
	HBRUSH m_BrushOld ,m_BrushRed ,m_BrushBlue ,m_BrushTransparent ;
	bool m_isFastRender ;
	int m_ticks ;
	int m_Generation ;
	const int& g_cxClient ,g_cyClient ;
	void PlotStats(HDC surface );
	void CreateSensors(vector<SPoint>& sensors ,int NumSensors ,double range );

public :
	Controller(const int& cxClient ,const int& cyClient );
	~Controller(void );
	bool Update(void );
	void Render(HDC );
	void PushKey(int );

	void WorldTransform(vector<SPoint>& ,S2DVector pos ,double rotation ,double scale );
	bool FastRender(void )const {return m_isFastRender ;}
	void FastRender(bool arg ){m_isFastRender =arg ;}
	void FastRenderToggle(void ){m_isFastRender =!m_isFastRender ;}
};

#endif