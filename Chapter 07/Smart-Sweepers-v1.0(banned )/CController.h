#ifndef __CCONTROLLER__DEF
#define __CCONTROLLER__DEF
#include "LogSpawner.h"
using RedContritio::LogSpawner ;
extern LogSpawner loger ;
#include <windows.h>
#include "S2DVector.h"
#include "S2DMatrix.h"

#include "utils.hpp"

#include <vector>
using std::vector ;

#include "CMineSweeper.h"
#include "CGenAlg.h"
#include "CParams.h"

class CCONTROLLER
{
	private :
		vector<SGENOME> vecPopulation ;
		vector<CMineSweeper> vecSweepers ;
		vector<S2DVector> vecMines ;

		CGeneticAlgorithm *pGA ;

		int NumSweepers ;
		int NumMines ;
		int NumWeightsInNN ;

		vector<SPoint> SweeperVB ;
		vector<SPoint> MineVB ;

		vector<double> vecAverageFitness ;
		vector<double> vecBestFitness ;

		HPEN PenRed ;
		HPEN PenGreen ;
		HPEN PenBlue ;
		HPEN PenOld ;

		bool isFastRender ;
		int Ticks ;
		int Generation ;

		void PlotStats(HDC surface );
	public :
		CCONTROLLER(void );
		~CCONTROLLER(void );
		bool Update(void );
		void Render(HDC );
		bool FaseRender()const {return isFastRender ;}
		void FaseRender(bool arg ){isFastRender = arg ;}
		void FaseRenderToggle(void ){isFastRender = !isFastRender ;}
};
#endif