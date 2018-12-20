#include "Controller.h"
#include "CParam.h"
#include "LogSpawner.h"
using RedContritio::LogSpawner ;

extern LogSpawner loger ;

#pragma warning(push)
#pragma warning(disable:4996)
static void RunTimeRenderTest(HDC surface );

static const int NumSweeperVerts =16 ;
static const SPoint sweeper[NumSweeperVerts] ={
SPoint(-1.0 ,-1.0 ),SPoint(-1.0 ,+1.0 ),SPoint(-0.5 ,+1.0 ),SPoint(-0.5 ,-1.0 ),

SPoint(+0.5 ,-1.0 ),SPoint(+1.0 ,-1.0 ),SPoint(+1.0 ,+1.0 ),SPoint(+0.5 ,+1.0 ),

SPoint(-0.5 ,-0.5 ),SPoint(+0.5 ,-0.5 ),SPoint(-0.5 ,+0.5 ),SPoint(-0.25,+0.5 ),
SPoint(-0.25,+1.75),SPoint(+0.25,+1.75),SPoint(+0.25,+0.5 ),SPoint(+0.5 ,+0.5 )
};

static const int NumObjectVerts =42 ;// 40 ?
static const SPoint objects[NumObjectVerts] ={
SPoint(80 ,60 ),SPoint(200,60 ),SPoint(200,60 ),SPoint(200,100),SPoint(200,100),SPoint(160,100),
SPoint(160,100),SPoint(160,200),SPoint(160,200),SPoint(80 ,200),SPoint(80 ,200),SPoint(80 ,60 ),
SPoint(250,100),SPoint(300,40 ),SPoint(300,40 ),SPoint(350,100),SPoint(350,100),SPoint(250,100),
SPoint(220,180),SPoint(320,180),SPoint(320,180),SPoint(320,300),SPoint(320,300),SPoint(220,300),
SPoint(220,300),SPoint(220,180),SPoint(12 ,15 ),SPoint(380,15 ),SPoint(380,15 ),SPoint(380,360),
SPoint(380,360),SPoint(12 ,360),SPoint(12 ,360),SPoint(12 ,340),SPoint(12 ,340),SPoint(100,290),
SPoint(100,290),SPoint(12 ,240),SPoint(12 ,240),SPoint(12 ,15 )
};



Controller::Controller(const int & cxClient, const int & cyClient):g_cxClient(cxClient ),g_cyClient(cyClient )
{
	m_isFastRender =false ;
	m_NumSweepers =CPARAM::NumSweepers ;
	m_Generation =0 ;
	m_ticks =0 ;

	for(int i=0 ;i<m_NumSweepers ;i++ )
	{
		m_sweepers.push_back(MineSweeper());
	}

	m_NumWeightsInNN =m_sweepers[0].GetNumberOfWeights() ;

	vector<int> SplitPoints =m_sweepers[0].CalculateSplitPoints();
	m_pGA =new CGENALG(m_NumSweepers ,CPARAM::MutationRate ,CPARAM::CrossoverRate ,m_NumWeightsInNN ,SplitPoints );

	m_population =m_pGA ->GetChromos() ;

	for(int i=0 ;i<m_NumSweepers ;i++ )
	{
		m_sweepers[i].SetWeights(m_population[i].m_weights );
	}

	m_PenRed =CreatePen(PS_SOLID ,1 ,RGB(0xFF ,0x00 ,0x00 ));
	m_PenGreen =CreatePen(PS_SOLID ,1 ,RGB(0x00 ,0xFF ,0x00 ));
	m_PenBlue =CreatePen(PS_SOLID ,1 ,RGB(0x00 ,0x00 ,0xFF ));
	m_PenGrayDot =CreatePen(PS_DOT ,1 ,RGB(0xC8 ,0xC8 ,0xC8 ));
	m_PenRedDot =CreatePen(PS_DOT ,1 ,RGB(0xFA ,0xC8 ,0xC8 ));
	m_PenOld =NULL ;

	m_BrushRed =CreateSolidBrush(RGB(0x00 ,0x00 ,0xF4 ));
	m_BrushBlue =CreateSolidBrush(RGB(0x95 ,0x00 ,0x00 ));
	m_BrushTransparent =(HBRUSH )GetStockObject(NULL_BRUSH );
	m_BrushOld =NULL ;

	for(int i=0 ;i<NumSweeperVerts ;i++ )
	{
		m_SweeperVB.push_back(sweeper[i] );
	}
	for(int i=0 ;i<NumObjectVerts ;i++ )
	{
		m_ObjectsVB.push_back(objects[i] );
	}
}

Controller::~Controller(void )
{
	if(m_pGA )
	{
		delete m_pGA ;
		m_pGA =NULL ; // useless 
	}

	DeleteObject(m_PenRed );
	DeleteObject(m_PenGreen );
	DeleteObject(m_PenBlue );
	DeleteObject(m_PenRedDot );
	DeleteObject(m_PenGrayDot );

	DeleteObject(m_BrushRed );
	DeleteObject(m_BrushBlue );
}

bool Controller::Update(void)
{
	if(ProcessKeys())
	{
		loger.puts("unknown error in Controller::Update : ProcessKeys Failed \n");
	}

	if(m_ticks++ <CPARAM::NumTicks )
	{
		for(int i=0 ;i<m_NumSweepers ;i++ )
		{
			if(!m_sweepers[i].Update(m_ObjectsVB ))
			{
				loger.puts("Wrong amount of NN inputs .");
				return false ;
			}
		}
	}
	else
	{
		for(int i=0 ;i<m_sweepers.size() ;i++ )
		{
			m_sweepers[i].EndRunCalculations();

			m_population[i].m_fitness =m_sweepers[i].Fitness() ;
		}

		m_AverageFitness.push_back(fabs(m_pGA->AverageFitness()));
		m_BestFitness.push_back(fabs(m_pGA->BestFitness()));

		m_Generation ++ ;
		m_ticks =0 ;

		m_population =m_pGA->Epoch(m_population );

		for(int i=0 ;i<m_NumSweepers ;i++ )
		{
			m_sweepers[i].SetWeights(m_population[i].m_weights );
			m_sweepers[i].Reset();
		}
	}
	return true ;
}

int Controller::ProcessKeys(void )
{
	while(!m_keys.empty())
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
	return 0 ;
}

void Controller::Render(HDC surface )
{
//	RunTimeRenderTest(surface );
	char ts[64] ;
	sprintf(ts ,"Generation : %d ",m_Generation );
	TextOut(surface ,5 ,0 ,ts ,strlen(ts ));

	if(!m_isFastRender )
	{
		m_sweepers[0].RenderPenalties(surface );
		for(int i=0 ;i<NumObjectVerts ;i+=2 )
		{
			MoveToEx(surface ,m_ObjectsVB[i].x ,m_ObjectsVB[i].y ,NULL );
			LineTo(surface ,m_ObjectsVB[i+1].x ,m_ObjectsVB[i+1].y );
		}

		m_PenOld =(HPEN )SelectObject(surface ,m_PenGrayDot );

		const HPEN hppen[4] ={m_PenGrayDot ,m_PenOld ,m_PenRedDot ,m_PenRedDot };
		for(size_t i=0 ;i<m_sweepers.size() ;i++ )
		{
			int pisElite =1 *(i <CPARAM::NumElite );
			int pisCollided =2*m_sweepers[i].Collided() ;
			SelectObject(surface ,hppen[pisElite |pisCollided ]);

			vector<SPoint> sweeperVB =m_SweeperVB ;
			m_sweepers[i].WorldTransform(sweeperVB ,m_sweepers[i].Scale());

#define MTX(i) MoveToEx(surface ,(int)sweeperVB[i].x ,(int)sweeperVB[i].y ,NULL )
#define LT(i) LineTo(surface ,(int)sweeperVB[i].x ,(int)sweeperVB[i].y )

			MTX(0 );
			for(int vert=1 ;vert <4 ;vert ++ )LT(vert );
			LT(0 );

			MTX(4 );
			for(int vert=5 ;vert <8 ;vert ++ )LT(vert );
			LT(4 );
			LT(9 );
			MTX(10 );
			for(int vert=11 ;vert <16 ;vert++ )LT(vert );
#undef MTX
#undef MTX
		}

		for(int i=0 ;i<CPARAM::NumElite ;i++ )
		{
			vector<SPoint> transSensors =m_sweepers[i].Sensors();
			vector<double> SensorReadings =m_sweepers[i].SensorReadings();
			for(int j=0 ;j<transSensors.size() ;j++ )
			{
				if(SensorReadings.size()>j && SensorReadings[j] >0 )
				{
					SelectObject(surface ,m_PenRed );
				}
				else
				{
					SelectObject(surface ,m_PenGrayDot );
				}

				if(!(fabs(m_sweepers[i].Position().x -transSensors[j].x ) >(CPARAM::SensorRange+1 )) ||
					(fabs(m_sweepers[i].Position().y -transSensors[j].y ) >(CPARAM::SensorRange+1 )) )
				{
					MoveToEx(surface ,(int)m_sweepers[i].Position().x ,(int)m_sweepers[i].Position().y ,NULL );
					LineTo(surface ,(int)transSensors[j].x ,(int)transSensors[j].y );
				}
			}
		}

		SelectObject(surface ,m_PenOld );
	}
	else
	{
		PlotStats(surface );
	}
}

void Controller::PushKey(int vk )
{
	m_keys.push(vk );
}

void Controller::WorldTransform(vector<SPoint>& points , S2DVector pos, double rotation, double scale)
{
	C2DMATRIX::TransformSPoints(points ,pos.x ,pos.y ,scale ,scale ,rotation );
}


void Controller::PlotStats(HDC surface)
{
	char ts[64] ;
	sprintf(ts ,"Best Fitness : %lf ",m_pGA->BestFitness());
	TextOut(surface ,5 ,20 ,ts ,strlen(ts ));

	sprintf(ts ,"Average Fitness : %lf ",m_pGA->AverageFitness());
	TextOut(surface ,5 ,40 ,ts ,strlen(ts ));

	double HSlice =g_cxClient/(m_Generation+1.0 );
	double VSlice =g_cyClient/((m_pGA->BestFitness()+1)*1.5 );

	double x =0 ;

	m_PenOld =(HPEN )SelectObject(surface ,m_PenRed );
	MoveToEx(surface ,0 ,g_cyClient ,NULL );

	for(int i=0 ;i<m_BestFitness.size() ;i++ )
	{
		LineTo(surface ,x ,(g_cyClient -VSlice*m_BestFitness[i]));
		x += HSlice ;
	}

	x =0 ;
	SelectObject(surface ,m_PenBlue );
	MoveToEx(surface ,0 ,g_cyClient ,NULL );

	for(int i=0 ;i<m_AverageFitness.size() ;i++ )
	{
		LineTo(surface ,x ,(g_cyClient -VSlice*m_AverageFitness[i]));
		x += HSlice ;
	}

	SelectObject(surface ,m_PenOld );
}

static void RunTimeRenderTest(HDC surface )
{
	extern int cxClient ;
	extern int cyClient ;
	static const int MaxTicks =100 ;
	static const double MaxV =15 ;
	static int ticks =MaxTicks ;
	static double px=0 ,py=0 ;
	static double vx=0 ,vy=0 ;
	static double ax=0 ,ay=0 ;
	if(ticks )
	{
		ticks -- ;
		vx += ax ,vy += ay ;
		px += vx ,py += vy ;
		Clamp(px ,0 ,cxClient );
		Clamp(py ,0 ,cyClient );
		Ellipse(surface ,(int)px ,(int)py ,(int)px+5 ,(int)py+5 );
	}
	else
	{
		ticks =MaxTicks ;
		ax =0.1*(RandFloat()-0.5 ) ;
		ay =0.1*(RandFloat()-0.5 ) ;
		if(vx*vx +vy*vy >MaxV*MaxV )
		{
			ax =-vx*0.01*1.2 ;
			ay =-vy*0.01*1.2 ;
		}
//		if(vx >MaxV &&ax>0 )ax =-ax ;
//		if(vx <-MaxV && ax <0 )ax =-ax ;
//		if(vy >MaxV &&ay>0 )ay =-ay ;
//		if(vy <-MaxV && ay <0 )ay =-ay ;
	}
}