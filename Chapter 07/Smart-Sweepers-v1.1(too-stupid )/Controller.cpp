#include "Controller.h"
#include "utils.hpp"
#include <cstdlib>
#include <cassert>

#pragma warning(push)

static const unsigned int Colors[] ={RGB(0x00 ,0x00 ,0x00 ),
									 RGB(0xFF ,0x00 ,0x00 ),RGB(0x00 ,0xFF ,0x00 ),RGB(0x00 ,0x00 ,0xFF ),
									 RGB(0xFF ,0xFF ,0x00 ),RGB(0xFF ,0x00 ,0xFF ),RGB(0x00 ,0xFF ,0xFF ),
									 RGB(0xFF ,0xFF ,0X00 )};

static const int NumSweeperVerts =16 ;
static const SPoint sweeperVB[NumSweeperVerts] ={
SPoint(-1.0 ,-1.0 ),SPoint(-1.0 ,+1.0 ),SPoint(-0.5 ,+1.0 ),SPoint(-0.5 ,-1.0 ),
SPoint(+0.5 ,-1.0 ),SPoint(+1.0 ,-1.0 ),SPoint(+1.0 ,+1.0 ),SPoint(+0.5 ,+1.0 ),
SPoint(-0.5 ,-0.5 ),SPoint(+0.5 ,-0.5 ),
SPoint(-0.5 ,+0.5 ),SPoint(-0.25,+0.5 ),SPoint(-0.25,1.75 ),SPoint(0.25 ,1.75 ),SPoint(0.25 ,+0.5 ),SPoint(+0.5 ,+0.5 )
};

static const int NumMineVerts =4 ;
static const SPoint mineVB[NumMineVerts] ={SPoint(-1 ,-1 ),SPoint(-1 ,+1 ),SPoint(+1 ,+1 ),SPoint(+1 ,-1 )};

template<typename T >
int FindElement(T *elements ,int Num ,T value )
{
	for(int i=0 ;i<Num ;i++ )
	{
		if(elements[i] ==value )return i ;
	}
	return -1 ;
}

namespace RedContritio{

Controller::Controller(const int &cxClient ,const int &cyClient ):g_cxClient(cxClient ),g_cyClient(cyClient ),
m_EntityUpdate(NULL ),m_EntityRender(NULL ),m_NumSweepers(CParam::NumOfSweepers ),m_pGA(NULL ),m_Ticks(0 ),
m_NumMines(CParam::NumOfMines ),m_Generations(0 )
{
	for(int i=0 ;i<m_NumSweepers ;i++ )
	{
		m_Sweepers.push_back(MineSweeper());
	}
	m_NumWeightsInNN =m_Sweepers[0].GetNumOfWeights();
	vector<int> SplitPoints =m_Sweepers[0].CalculateSplitPoints();

	m_pGA =new CGenAlg(m_NumSweepers ,CParam::MutationRate ,CParam::CrossoverRate ,
										m_NumWeightsInNN ,SplitPoints );

	m_Population =m_pGA->GetChromos();

	for(int i=0 ;i<m_NumSweepers ;i++ )
	{
		m_Sweepers[i].SetWeights(m_Population[i].m_weights );
	}
	
	for(int i=0 ;i<m_NumMines ;i++ )
	{
		m_Mines.push_back(S2DVector(RandFloat()*g_cxClient ,RandFloat()*g_cyClient ));
	}

	CreatePaintTools(Colors ,sizeof(Colors )/sizeof(Colors[0] ));

	for(int i=0 ;i<NumSweeperVerts ;i++ )m_SweeperVB.push_back(sweeperVB[i] );
	for(int i=0 ;i<NumMineVerts ;i++ )m_MineVB.push_back(mineVB[i] );
}

Controller::~Controller(void )
{
	if(m_pGA )
	{
		delete m_pGA ;
		m_pGA =NULL ;
	}

	DeletePaintTools();
}

bool Controller::Update(void)
{
	bool TrueFlag = true ;
	unsigned int arg_passin =0 ;
	while(m_keys.size() >0 )
	{
		switch(m_keys.front())
		{
			case 'F' :
			{
				m_isFastRender =!m_isFastRender ;
				break ;
			}
			default :
			{
				break ;
			}
		}
		m_keys.pop();
	}

	if(m_Ticks++ <CParam::NumOfTicks )
	{
		for(int i=0 ;i<m_NumSweepers ;i++ )
		{
			if(m_Sweepers[i].Update(m_Mines ) ==false )
			{
				MessageBox(NULL ,"Error in Controller to update sweeper" ,"Error" ,MB_OK );
				return false ;
			}

			int GrabHit =m_Sweepers[i].CheckCrashToMine(m_Mines ,CParam::MineScale );

			if(GrabHit >= 0 )
			{
				m_Sweepers[i].IncrementFitness();
				m_Mines[GrabHit] =S2DVector(RandFloat()*g_cxClient ,RandFloat()*g_cyClient );
			}

			m_Population[i].m_Fitness =m_Sweepers[i].Fitness();
		}
	}
	else
	{
		m_AvFitness.push_back(m_pGA->AverageFitness());
		m_BestFitness.push_back(m_pGA->BestFitness());

		++m_Generations ;
		m_Ticks =0 ;

		m_Population =m_pGA->Epoch(m_Population );

		for(int i=0 ;i<m_NumSweepers ;i++ )
		{
			m_Sweepers[i].SetWeights(m_Population[i].m_weights );
			m_Sweepers[i].Reset();
		}
	}
	return true ;
}

#pragma warning(disable:4996)
void Controller::Render(HDC surface)
{
	char tstr[30] ;
	sprintf(tstr ,"Generation :%s%d" ,"        " ,m_Generations );
	TextOut(surface ,5 ,0 ,tstr ,strlen(tstr ));

	if(!m_isFastRender )
	{
		HPEN& PenGreen =m_PenSolid[FindElement<unsigned int>(m_RGBs ,m_NumOfRGBs ,RGB(0x00 ,0xFF ,0x00 ))] ;
		HPEN& PenRed =m_PenSolid[FindElement<unsigned int>(m_RGBs ,m_NumOfRGBs ,RGB(0xFF ,0x00 ,0x00 ))] ;
		m_PenOld =(HPEN )SelectObject(surface ,PenGreen );
		for(int i=0 ;i<m_NumMines ;i++ )
		{
			vector<SPoint> tmineVB =m_MineVB ;
			C2DMATRIX::TransformSPoints(tmineVB ,m_Mines[i].x ,m_Mines[i].y ,CParam::MineScale ,CParam::MineScale ,0 );
			MoveToEx(surface ,(int)tmineVB[0].x ,(int)tmineVB[0].y ,NULL );
			for(int vert=1 ;vert <tmineVB.size() ;vert++ )
			{
				LineTo(surface ,(int)tmineVB[vert].x ,(int)tmineVB[vert].y );
			}
			LineTo(surface ,(int)tmineVB[0].x ,(int)tmineVB[0].y );
		}

		SelectObject(surface ,PenRed );

		for(int i=0 ;i<m_NumSweepers ;i++ )
		{
			if(i ==CParam::NumOfElite )
			{
				SelectObject(surface ,m_PenOld );
			}
			vector<SPoint> tsweeperVB =m_SweeperVB ;
			m_Sweepers[i].WorldTransform(tsweeperVB );

			MoveToEx(surface ,(int)tsweeperVB[0].x ,(int)tsweeperVB[0].y ,NULL );
			for(int vert =1 ;vert <4 ;vert++ )
			{
				LineTo(surface ,(int)tsweeperVB[vert].x ,(int)tsweeperVB[vert].y );
			}
			LineTo(surface ,(int)tsweeperVB[0].x ,(int)tsweeperVB[0].y );

			MoveToEx(surface ,(int)tsweeperVB[4].x ,(int)tsweeperVB[4].y ,NULL );
			for(int vert =5 ;vert <8 ;vert++ )
			{
				LineTo(surface ,(int)tsweeperVB[vert].x ,(int)tsweeperVB[vert].y );
			}
			LineTo(surface ,(int)tsweeperVB[4].x ,(int)tsweeperVB[4].y );

			MoveToEx(surface ,(int)tsweeperVB[8].x ,(int)tsweeperVB[8].y ,NULL );
			LineTo(surface ,(int)tsweeperVB[9].x ,(int)tsweeperVB[9].y );

			MoveToEx(surface ,(int)tsweeperVB[10].x ,(int)tsweeperVB[10].y ,NULL );

			for(int vert =11 ;vert <16 ;vert ++ )
			{
				LineTo(surface ,(int)tsweeperVB[vert].x ,(int)tsweeperVB[vert].y );
			}

			MoveToEx(surface ,(int)m_Sweepers[i].Position().x ,(int)m_Sweepers[i].Position().y ,NULL );
			LineTo(surface ,(int)m_Mines[m_Sweepers[i].ClosestMineID()].x ,(int)m_Mines[m_Sweepers[i].ClosestMineID()].y );
		}

		SelectObject(surface ,m_PenOld );
	}
	else
	{
		PlotStatus(surface );
	}

	return ;
}

void Controller::PushKey(int vk )
{
	m_keys.push(vk );
	return ;
}



#pragma warning(disable:4018)
void Controller::CreatePaintTools(const unsigned int *RGBs ,int NumOfElements )
{
	m_NumOfRGBs =NumOfElements ;
	m_PenSolid =(HPEN *)malloc(m_NumOfRGBs *sizeof(HPEN ));
	m_BrushSolid =(HBRUSH *)malloc(m_NumOfRGBs *sizeof(HBRUSH ));
	m_RGBs =(unsigned int *)malloc(m_NumOfRGBs *sizeof(unsigned int ));
	memmove(m_RGBs ,RGBs ,NumOfElements *sizeof(unsigned int ));
	assert(m_PenSolid &&m_BrushSolid );
	for(int i=0 ;i<m_NumOfRGBs ;i++ )
	{
		m_PenSolid[i] =(HPEN )CreatePen(PS_SOLID ,1 ,RGBs[i] );
		m_BrushSolid[i] =(HBRUSH )CreateSolidBrush(RGBs[i] );
	}
	m_BrushTransparent =(HBRUSH )GetStockObject(NULL_BRUSH );
	return ;
}

void Controller::DeletePaintTools(void )
{
	for(int i=0 ;i<m_NumOfRGBs ;i++ )
	{
		if(m_PenSolid[i] )DeleteObject(m_PenSolid[i] );
		if(m_BrushSolid[i] )DeleteObject(m_BrushSolid[i] );
	}
	if(m_PenSolid )
	{
		free(m_PenSolid );
		m_PenSolid =NULL ;
	}
	if(m_BrushSolid )
	{
		free(m_PenSolid );
		m_PenSolid =NULL ;
	}
	return ;
}

bool(*Controller::SelectEntityUpdate(bool(*method )(void* ,unsigned)))(void* ,unsigned)
{
	bool (*tmpMethod)(void* ,unsigned ) =m_EntityUpdate ;
	m_EntityUpdate =method  ;
	return tmpMethod ;
}

bool (*(Controller::ReleaseEntityUpdate(void )))(void* ,unsigned )
{
	return SelectEntityUpdate(NULL );
}

void(*Controller::SelectEntityRender(void (*method )(void* ,HDC)))(void* ,HDC)
{
	void (*tmpMethod)(void* ,HDC ) =m_EntityRender ;
	m_EntityRender =method  ;
	return tmpMethod ;
}

void(*Controller::ReleaseEntityRender(void))(void* ,HDC)
{
	return SelectEntityRender(NULL );
}
void Controller::PlotStatus(HDC surface)
{
	char str[50] ;
	sprintf(str ,"Best Fitness : %g " ,m_pGA->BestFitness());
	TextOut(surface ,5 ,20 ,str ,strlen(str ));
	sprintf(str ,"Average Fitness : %g " ,m_pGA->AverageFitness());
	TextOut(surface ,5 ,40 ,str ,strlen(str ));
	double HSlice =g_cxClient/(m_Generations+1.0 );
	double VSlice =g_cyClient/(1.5*(m_pGA->BestFitness()+1.0));
	double x =0 ;
	HPEN& PenRed =m_PenSolid[FindElement<unsigned int>(m_RGBs ,m_NumOfRGBs ,RGB(0xFF ,0x00 ,0x00 ))] ;
	HPEN& PenBlue =m_PenSolid[FindElement<unsigned int>(m_RGBs ,m_NumOfRGBs ,RGB(0x00 ,0x00 ,0xFF ))] ;
	m_PenOld =(HPEN)SelectObject(surface ,PenRed );

	MoveToEx(surface ,0 ,g_cyClient ,NULL );

	for(int i=0 ;i<m_BestFitness.size() ;i++ )
	{
		LineTo(surface ,(int)x ,(int)(g_cyClient-VSlice*m_BestFitness[i]));
		x += HSlice ;
	}

	x =0 ;
	SelectObject(surface ,PenBlue );
	MoveToEx(surface ,0 ,g_cyClient ,NULL );
	for(int i=0 ;i<m_AvFitness.size() ;i++ )
	{
		LineTo(surface ,(int)x ,(int)(g_cyClient-VSlice*m_AvFitness[i]));
		x += HSlice ;
	}
	SelectObject(surface ,m_PenOld );
}
#pragma warning(pop)

}