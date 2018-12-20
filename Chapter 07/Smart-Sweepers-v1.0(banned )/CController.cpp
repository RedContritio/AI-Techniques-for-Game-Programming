#include "CController.h"
extern int cxClient ,cyClient ;
const int	 NumSweeperVerts = 16;
const SPoint sweeper[NumSweeperVerts] = 
	 {SPoint(-1, -1),SPoint(-1, 1),SPoint(-0.5, 1),SPoint(-0.5, -1),

	  SPoint(0.5, -1),SPoint(1, -1),SPoint(1, 1),SPoint(0.5, 1),

	  SPoint(-0.5, -0.5),SPoint(0.5, -0.5),

	  SPoint(-0.5, 0.5),SPoint(-0.25, 0.5),SPoint(-0.25, 1.75),SPoint(0.25, 1.75),SPoint(0.25, 0.5),SPoint(0.5, 0.5)};

const int NumMineVerts = 4;
const SPoint mine[NumMineVerts] = 
	 {SPoint(-1, -1),SPoint(-1, 1),SPoint(1, 1),SPoint(1, -1)};

CCONTROLLER::CCONTROLLER(void ):NumSweepers(CPARAMETERS::NumSweepers ),pGA(NULL ),isFastRender(false ),Ticks(0 ),
								NumMines(CPARAMETERS::NumMines ),Generation(0 )
{
	for(int i=0 ;i<NumSweepers ;i++ )
	{
		vecSweepers.push_back(CMineSweeper());
	}

	NumWeightsInNN = vecSweepers[0].GetNumOfWeights() ;

	pGA = new CGeneticAlgorithm(NumSweepers ,CPARAMETERS::MutationRate ,CPARAMETERS::CrossoverRate ,NumWeightsInNN );

	vecPopulation = pGA->GetChromos();

	for(int i=0 ;i<NumSweepers ;i++ )
	{
		vecSweepers[i].PutWeights(vecPopulation[i].Weights );
	}

	for(int i=0 ;i<NumMines ;i++ )
	{
		vecMines.push_back(S2DVector(RandFloat() * cxClient ,RandFloat() * cyClient ));
	}

	PenRed = CreatePen(PS_SOLID ,1 ,RGB(0xFF ,0 ,0 ));
	PenGreen = CreatePen(PS_SOLID ,1 ,RGB(0 ,0xFF ,0 ));
	PenBlue = CreatePen(PS_SOLID ,1 ,RGB(0 ,0 ,0xFF ));
	PenOld = NULL ;

	for(int i=0 ;i<NumSweeperVerts ;i++ )
	{
		SweeperVB.push_back(sweeper[i] );
	}
	for(int i=0 ;i<NumMineVerts ;i++ )
	{
		MineVB.push_back(mine[i] );
	}
}

CCONTROLLER::~CCONTROLLER(void )
{
	if(pGA )
	{
		delete pGA ;
	}

	DeleteObject(PenRed );
	DeleteObject(PenGreen );
	DeleteObject(PenBlue );
	DeleteObject(PenOld );
}

bool CCONTROLLER::Update(void )
{
	if(Ticks++ < CPARAMETERS::NumMaxTicks )
	{
		for(int i=0 ;i<NumSweepers ;i++ )
		{
			if(!(vecSweepers[i].Update(vecMines )))
			{
				loger.printf("Wrong Amount of NN inputs!\n");
				MessageBox(NULL ,"Failed to Transmiss Nerve Impulse ","Error" ,MB_OK );
				return false ;
			}

			int GrabHit = vecSweepers[i].CheckForMine(vecMines ,CPARAMETERS::MineScale );

			if(GrabHit >= 0 )
			{
				vecSweepers[i].IncrementFitness();
				vecMines[GrabHit] = S2DVector(RandFloat() * cxClient ,RandFloat() * cyClient ) ;
				vecPopulation[i].Fitness = vecSweepers[i].Fitness() ;
			}
		}
	}
	else
	{
		vecAverageFitness.push_back(pGA->AverageFitness());
		vecBestFitness.push_back(pGA->BestFitness());

		Generation ++ ;
		Ticks = 0  ;

		vecPopulation = pGA->Epoch(vecPopulation );
		for(int i=0 ;i<NumSweepers ;i++ )
		{
			vecSweepers[i].PutWeights(vecPopulation[i].Weights );
			vecSweepers[i].Reset();
		}
	}
	return true ;
}

void CCONTROLLER::Render(HDC hdc )
{
	char strGeneration[32] ;
	sprintf_s(strGeneration ,"Generation : %d ",Generation );
	TextOut(hdc ,5 ,0 ,strGeneration ,strlen(strGeneration ));

	if(isFastRender )
	{
		PlotStats(hdc );
	}
	else
	{
		PenOld = (HPEN)SelectObject(hdc ,PenGreen );
		for(int i=0 ;i<NumMines ;i++ )
		{
			vector<SPoint> mineVB = MineVB ;
			C2DMATRIX matTransform ;
			matTransform.Scale(CPARAMETERS::MineScale ,CPARAMETERS::MineScale );
			matTransform.Move(vecMines[i].x ,vecMines[i].y );
			for(size_t j=0 ;j<MineVB.size() ;j++ )
			{
				mineVB[j] = SPoint(MineVB[j].x * matTransform.matrix.se._00 + MineVB[j].y * matTransform.matrix.se._10 + matTransform.matrix.se._20 ,
							  	   MineVB[j].x * matTransform.matrix.se._01 + MineVB[j].y * matTransform.matrix.se._11 + matTransform.matrix.se._21 );
			}
			MoveToEx(hdc ,(int)mineVB[0].x ,(int)mineVB[0].y ,NULL );
			for(size_t j=1 ;j<mineVB.size() ;j++ )
			{
				LineTo(hdc ,(int)mineVB[j].x ,(int)mineVB[j].y );
			}
			LineTo(hdc ,(int)mineVB[0].x ,(int)mineVB[0].y );
		}

		SelectObject(hdc ,PenRed );
		for(int i=0 ;i<NumSweepers ;i++ )
		{
			if(i==CPARAMETERS::NumElites )
			{
				SelectObject(hdc ,PenOld );
			}

			vector<SPoint> sweeperVB = SweeperVB ;

			C2DMATRIX matTransform ;
			matTransform.Scale(CPARAMETERS::SweeperScale ,CPARAMETERS::SweeperScale );
			matTransform.Rotate(vecSweepers[i].Rotation());
			matTransform.Move(vecSweepers[i].Position().x ,vecSweepers[i].Position().y );
			for(size_t j=0 ;j<SweeperVB.size() ;j++ )
			{
				sweeperVB[j] = SPoint(SweeperVB[j].x * matTransform.matrix.se._00 + SweeperVB[j].y * matTransform.matrix.se._10 + matTransform.matrix.se._20 ,
									  SweeperVB[j].x * matTransform.matrix.se._01 + SweeperVB[j].y * matTransform.matrix.se._11 + matTransform.matrix.se._21 );
			}

			MoveToEx(hdc ,(int)sweeperVB[0].x ,(int)sweeperVB[0].y ,NULL );
			for(int j=1 ;j<4 ;j++ )
			{
				LineTo(hdc ,(int)sweeperVB[j].x ,(int)sweeperVB[j].y );
			}
			LineTo(hdc ,(int)sweeperVB[0].x ,(int)sweeperVB[0].y );

			MoveToEx(hdc ,(int)sweeperVB[4].x ,(int)sweeperVB[4].y ,NULL );
			for(int j=5 ;j<8 ;j++ )
			{
				LineTo(hdc ,(int)sweeperVB[j].x ,(int)sweeperVB[j].y );
			}
			LineTo(hdc ,(int)sweeperVB[4].x ,(int)sweeperVB[4].y );

			MoveToEx(hdc ,(int)sweeperVB[8].x ,(int)sweeperVB[8].y ,NULL );
			LineTo(hdc ,(int)sweeperVB[9].x ,(int)sweeperVB[9].y );

			MoveToEx(hdc ,(int)sweeperVB[10].x ,(int)sweeperVB[10].y ,NULL );

			for(int j=11 ;j<16 ;j++ )
			{
				LineTo(hdc ,(int)sweeperVB[j].x ,(int)sweeperVB[j].y );
			}

		}
		SelectObject(hdc ,PenOld );
	}
}

void CCONTROLLER::PlotStats(HDC surface )
{
	char strFitness[32] ;
	sprintf_s(strFitness ,"BestFitness : %g ",pGA->BestFitness() );
	TextOut(surface ,5 ,20 ,strFitness ,strlen(strFitness ));
	sprintf_s(strFitness ,"Average Fitness : %g ",pGA->AverageFitness() );
	TextOut(surface ,5 ,40 ,strFitness ,strlen(strFitness ));
	double HSlice = cxClient / (Generation + 1.0 );
	double VSlice = cyClient / ((pGA->BestFitness()+1.0)*2);
	double x = 0 ;
	PenOld = (HPEN )SelectObject(surface ,PenRed );
	MoveToEx(surface ,0 ,cyClient ,NULL );
	for(size_t i=0 ;i<vecBestFitness.size() ;i++ )
	{
		LineTo(surface ,(int)x ,(int)(cyClient - VSlice * vecBestFitness[i]) );
		x += HSlice ;
	}

	SelectObject(surface ,PenBlue );
	MoveToEx(surface ,0 ,cyClient ,NULL );
	x = 0 ;
	for(size_t i=0 ;i<vecAverageFitness.size() ;i++ )
	{
		LineTo(surface ,(int)x ,(int)(cyClient - VSlice * vecAverageFitness[i]) );
		x += HSlice ;
	}
	SelectObject(surface ,PenOld );
}