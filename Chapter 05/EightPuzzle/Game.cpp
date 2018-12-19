#include "Game.h"

// Experience told me , the worst situation is : 8 7 6 5 4 3 2 1 0
#pragma warning(disable : 4018 4996)
const double SQRT[] = 
				{0.000 ,1.000 ,1.414 ,1.732 ,2.000 ,
				 2.236 ,2.449 ,2.646 ,2.828 ,3.000 ,
				 3.162 ,3.317 ,3.464 ,3.606 ,3.742 ,
				 3.873 ,4.000 ,4.123 ,4.243 ,4.359 ,
				 4.472 ,4.583 ,4.690 ,4.796 ,4.899 };
const int __SQUARE[] = {9 ,4 ,1 ,0 ,1 ,4 ,9 };
#define SQUARE_OFFSET 3
#define SQUARE(num) __SQUARE[(num)+SQUARE_OFFSET]
static inline double CalculateDistance(int a ,int b )
{
	return SQRT[SQUARE(a%3 - b%3) + SQUARE(a/3 - b/3)] ;
}

void GAME::CalculateFitness(void )
{
	double dis = 0 ;
	for(int i=0 ;i<9 ;i++ )
	{
		dis += CalculateDistance(i ,map[i] );
	}
	_Fitness = 25 - dis ;
	return ;
}

double GAME::BestFitness(void )const 
{
	return 25 ;
}

void GAME::VoidLocatation(void )
{
	for(pos=0 ;pos<9 ;pos++ )
	{
		if(!map[pos] )break ;
	}
	return ;
}

void GAME::SpawnRandomMap(void )
{
	int i=0 ; // fuck VC++
	for(i=0 ;i<9 ;i++ )map[i] = i ;
	for(i=0 ;i<9 ;i++ )
	{
		int p1 = RandInt(0 , 9-1 );
		int p2 = RandInt(0 , 9-1 );
		swap(map[p1] ,map[p2] );
	}
	return ;
}

bool GAME::CheckSolvable(void )
{
	int k = 0 ;
	for(int i=0 ;i<8 ;i++ )
	{
		for(int j=i+1 ;j<9 ;j++ )
		{
			if(map[i] && map[j] )
			{
				if(map[i] > map[j] )k++ ;
			}
		}
	}
	return !(k&1) ;
}


GAME::GAME(void )
{
	SpawnRandomMap( );
	while(!CheckSolvable())SpawnRandomMap( );
	VoidLocatation( );
}

GAME::GAME(int p )
{
	SpawnRandomMap( );
	VoidLocatation( );
	if(pos != p )
	{
		swap(map[pos] ,map[p] );
		pos = p ;
	}
}

GAME::~GAME(void )
{
	;
}

double GAME::Fitness(void )const 
{
	return _Fitness ;
}

int* GAME::GetMap(void )
{
	return map ;
}

bool GAME::Move(int operation )
{
	bool operated = false ;
	switch(operation )
	{
		case GAME_LEFT :
		{
			if(pos%3 != 2 )
			{
				swap(map[pos] ,map[pos+1] );
				pos += 1 ;
				operated = true ;
			}
			break ;
		}
		case GAME_UP :
		{
			if(pos/3 != 2 )
			{
				swap(map[pos] ,map[pos+3] );
				pos += 3 ;
				operated = true ;
			}
			break ;
		}
		case GAME_RIGHT :
		{
			if(pos%3 != 0 )
			{
				swap(map[pos] ,map[pos-1] );
				pos -= 1 ;
				operated = true ;
			}
			break ;
		}
		case GAME_DOWN :
		{
			if(pos/3 != 0 )
			{
				swap(map[pos] ,map[pos-3] );
				pos -= 3 ;
				operated = true ;
			}
			break ;
		}
	}
	return operated ;
}

GAME& ApplyOperations(GAME &game ,const vector<int> &ops )
{
	GAME& result = game ;
	for(int i=0 ;i<ops.size() ;i++)
	{
		result.Move(ops[i] );
	}
	result.CalculateFitness( );
	return result ;
}

void GameRenderer(HDC hdc ,GAME &game )
{
	const int margin = 30 ;
	const int interval = 10 ;
	const int HalfSideX = (cxClient - 2*margin - 2*interval )/6 ;
	const int HalfSideY = (cyClient - 2*margin - 2*interval )/6 ;
	const int center_x = cxClient/2 ;
	const int center_y = cyClient/2 ;

	const int Font_x = HalfSideX/3*2 ;
	const int Font_y = HalfSideY/3*2 ;

	game.CalculateFitness( );
	char Strscore[20] = "";
	sprintf(Strscore ,"score : %g%% ",100 * game.Fitness() / game.BestFitness() );
	TextOut(hdc ,center_x-5*strlen(Strscore),
			cyClient-20 ,Strscore ,strlen(Strscore ));

	HFONT NewFont = CreateFont(Font_x ,Font_y ,0 ,0 ,FW_THIN ,false ,false ,false ,CHINESEBIG5_CHARSET ,OUT_CHARACTER_PRECIS ,
							   CLIP_CHARACTER_PRECIS ,DEFAULT_QUALITY ,FF_MODERN ,"ËÎÌו" );
	HFONT OldFont = (HFONT)SelectObject(hdc ,NewFont );
	for(int i=0 ;i<3 ;i++ )
	{
		for(int j=0 ;j<3 ;j++ )
		{
			Rectangle(hdc ,center_x+(2*j-3)*HalfSideX+(j-1)*interval ,center_y+(2*i-3)*HalfSideY+(i-1)*interval ,
						   center_x+(2*j-1)*HalfSideX+(j-1)*interval ,center_y+(2*i-1)*HalfSideY+(i-1)*interval );
			if(game.GetMap()[3*i+j] )
			{
				char NumCode = '0' + game.GetMap()[3*i+j] ;
				TextOut(hdc ,center_x+(2*j-2)*HalfSideX+(j-1)*interval-Font_x/2 ,
					center_y+(2*i-2)*HalfSideY+(i-1)*interval-Font_y/2 ,&NumCode ,1 );
			}
		}
	}
	SelectObject(hdc ,OldFont );
	DeleteObject(NewFont );
}
