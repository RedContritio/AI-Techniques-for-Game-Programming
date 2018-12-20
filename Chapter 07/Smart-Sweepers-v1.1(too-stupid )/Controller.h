#ifndef CONTROLLER__INC
#define CONTROLLER__INC
#pragma once
#include <windows.h>
#include <queue>

#include "S2DVector.h"
#include "S2DMatrix.h"
#include "MineSweeper.h"
#include "GeneticAlgorithm.h"

namespace RedContritio{
class Controller
{
public :
	Controller(const int &cxClient ,const int &cyClient );
	~Controller(void );
	bool Update(void );
	void PushKey(int );
	void Render(HDC surface );
	bool FastRender(void )const {return m_isFastRender ;}
protected :
	vector<GENOME> m_Population ;
	vector<MineSweeper> m_Sweepers ;
	vector<S2DVector> m_Mines ;
	CGenAlg  *m_pGA ;

	int m_NumSweepers ;
	int m_NumMines ;
	int m_NumWeightsInNN ;

	vector<SPoint> m_SweeperVB ;
	vector<SPoint> m_MineVB ;
	vector<double> m_AvFitness ;
	vector<double> m_BestFitness ;

	bool m_isFastRender ;// unused 
	int m_Ticks ;
	int m_Generations ;

	const int &g_cxClient ;
	const int &g_cyClient ;
	HPEN *m_PenSolid ,m_PenOld ;
	unsigned int *m_RGBs ,m_NumOfRGBs ;
	HBRUSH *m_BrushSolid ,m_BrushTransparent ;
	std::queue<int> m_keys ;
	bool (*m_EntityUpdate)(void* ,unsigned );
	void (*m_EntityRender)(void* ,HDC );
	void CreatePaintTools(const unsigned int *RGBs ,int NumOfElements );
	void DeletePaintTools(void );
	bool (*SelectEntityUpdate(bool (*method )(void* ,unsigned )))(void* ,unsigned );
	bool (*ReleaseEntityUpdate(void ))(void* ,unsigned );
	void (*SelectEntityRender(void (*method )(void* ,HDC )))(void* ,HDC );
	void (*ReleaseEntityRender(void ))(void* ,HDC );
	void PlotStatus(HDC surface );
};
}
#endif