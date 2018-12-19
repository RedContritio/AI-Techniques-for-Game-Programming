#include "Map.h"
const int MAP::map[__MAP__HEIGHT][__MAP__WIDTH] 
				= {1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,
			   	   1 ,0 ,1 ,0 ,0 ,0 ,0 ,0 ,1 ,1 ,1 ,0 ,0 ,0 ,1 ,
				   8 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,1 ,1 ,1 ,0 ,0 ,0 ,1 ,
				   1 ,0 ,0 ,0 ,1 ,1 ,1 ,0 ,0 ,1 ,0 ,0 ,0 ,0 ,1 ,
				   1 ,0 ,0 ,0 ,1 ,1 ,1 ,0 ,0 ,0 ,0 ,0 ,1 ,0 ,1 ,
				   1 ,1 ,0 ,0 ,1 ,1 ,1 ,0 ,0 ,0 ,0 ,0 ,1 ,0 ,1 ,
				   1 ,0 ,0 ,0 ,0 ,1 ,0 ,0 ,0 ,0 ,1 ,1 ,1 ,0 ,1 ,
				   1 ,0 ,1 ,1 ,0 ,0 ,0 ,1 ,0 ,0 ,0 ,0 ,0 ,0 ,5 ,
				   1 ,0 ,1 ,1 ,0 ,0 ,0 ,1 ,0 ,0 ,0 ,0 ,0 ,0 ,1 ,
				   1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 };
const int MAP::width = __MAP__WIDTH ;
const int MAP::height = __MAP__HEIGHT ;
const int MAP::StartX = 14 ;
const int MAP::StartY = 7 ;
const int MAP::EndX = 0 ;
const int MAP::EndY = 2 ;
MAP::MAP(void )
{
	ResetMemory( );
}

double MAP::TestRoute(const vector<int> &vecPath ,MAP &player )
{
	int PosX = StartX ,PosY = StartY ;
	
	//printf("Testing : ");
	for(int i=0 ;i<vecPath.size() ;i++ )
	{
		//printf("(%d ,%d) ," ,PosX ,PosY );
		switch(vecPath[i] )
		{
			case __DIRECT__NORTH :
			{
				if(PosY-1 < 0 || map[PosY-1][PosX] == __BLOCK__WALL )
				{
					break ;
				}
				else
				{
					PosY -= 1 ;
				}
				break ;
			}
			case __DIRECT__SOUTH :
			{
				if(PosY+1 >= height || map[PosY+1][PosX] == __BLOCK__WALL )
				{
					break ;
				}
				else
				{
					PosY += 1 ;
				}
				break ;
			}
			case __DIRECT__EAST :
			{
				if(PosX+1 >= width || map[PosY][PosX+1] == __BLOCK__WALL )
				{
					break ;
				}
				else
				{
					PosX += 1 ;
				}
				break ;
			}
			case __DIRECT__WEST :
			{
				if(PosX-1 < 0 || map[PosY][PosX-1] == __BLOCK__WALL )
				{
					break ;
				}
				else
				{
					PosX -= 1 ;
				}
				break ;
			}

		}
		player.memory[PosY][PosX] = __BLOCK__PASSED ;
	}
	int DifX = abs(PosX - EndX );
	int DifY = abs(PosY - EndY );
	
	//printf("\n");
	return 1.0/(DifX+DifY+1) ;
}

void MAP::Render(const int cxClient ,const int cyClient ,HDC surface )
{
	const int boarder = 20 ;
	int BlockSizeX = (cxClient - 2*boarder )/width ;
	int BlockSizeY = (cyClient - 2*boarder )/height ;

	HBRUSH BlackBrush ,RedBrush ,OldBrush ;
	HPEN NullPen ,OldPen ;
	NullPen = (HPEN)GetStockObject(NULL_PEN );
	BlackBrush = (HBRUSH)GetStockObject(BLACK_BRUSH );
	RedBrush = CreateSolidBrush(RGB(255 ,0 ,0 ));

	OldBrush = (HBRUSH)SelectObject(surface ,BlackBrush );
	OldPen = (HPEN)SelectObject(surface ,NullPen );
	
	for(int i=0 ;i<width ;i++ )
	{
		for(int j=0 ;j<height ;j++ )
		{
			int left = boarder + i*BlockSizeX ;
			int right = left + BlockSizeX ;
			int top = boarder + j*BlockSizeY ;
			int bottom = top + BlockSizeY ;
			switch(map[j][i] )
			{
				case __BLOCK__WALL :
				{
					SelectObject(surface ,BlackBrush );
	
					Rectangle(surface ,left ,top ,right ,bottom );
					break ;
				}
				case __BLOCK__BEGIN :
				case __BLOCK__END :
				{
					SelectObject(surface ,RedBrush );

					Rectangle(surface ,left ,top ,right ,bottom );
					break ;
				}
			}
		}
	}
	SelectObject(surface ,OldPen );
	SelectObject(surface ,OldBrush );
	return ;
}

void MAP::MemoryRender(const int cxClient ,const int cyClient ,HDC surface )
{
	const int boarder = 20 ;
	int BlockSizeX = (cxClient - 2*boarder )/width ;
	int BlockSizeY = (cyClient - 2*boarder )/height ;

	HBRUSH GreyBrush ,OldBrush ;
	HPEN NullPen ,OldPen ;
	NullPen = (HPEN)GetStockObject(NULL_PEN );
	GreyBrush = (HBRUSH)GetStockObject(LTGRAY_BRUSH);

	OldBrush = (HBRUSH)SelectObject(surface ,GreyBrush );
	OldPen = (HPEN)SelectObject(surface ,NullPen );
	
	for(int i=0 ;i<width ;i++ )
	{
		for(int j=0 ;j<height ;j++ )
		{
			if(memory[j][i] == __BLOCK__PASSED )
			{
				int left = boarder + BlockSizeX * i ;
				int top = boarder + BlockSizeY * j ;
				int right = left + BlockSizeX ;
				int bottom = top + BlockSizeY ;
				Rectangle(surface ,left ,top ,right ,bottom ) ;
			}
		}
	}

	SelectObject(surface ,OldPen );
	SelectObject(surface ,OldBrush );
	return ;
}

void MAP::ResetMemory(void )
{
	memset(this->memory ,0 ,sizeof(memory ));
}