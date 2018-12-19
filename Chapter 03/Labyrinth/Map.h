#ifndef __MAP__DEF
#define __MAP__DEF
#include "defines.h"
#include <vector>
#include <Windows.h>
using std::vector ;
class MAP
{
	private :
		static const int map[__MAP__HEIGHT][__MAP__WIDTH] ;
		static const int width ;
		static const int height ;
		static const int StartX ,StartY ;
		static const int EndX ,EndY ;
	public :
		int memory[__MAP__HEIGHT][__MAP__WIDTH] ;
		MAP(void );
		double TestRoute(const vector<int> &,MAP & );
		void Render(const int ,const int ,HDC );
		void MemoryRender(const int,const int ,HDC );
		void ResetMemory(void );
};

#endif