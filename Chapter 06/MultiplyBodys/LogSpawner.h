#ifndef __LOG__SPAWN__RC__INC
#define __LOG__SPAWN__RC__INC
#include <cstdarg>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cassert>
#include <ctime>

namespace RedContritio
{
	class LOGSPAWNER
	{
		private :
			FILE* fout ;
			int SizeOfBuffer ;
			char* buffer ;
			int buflen ;
			mutable int counter ;
			void print(void );
			void load(const char* );

			int sputs(char* ,const char* );
			int lprintch(char* ,const char & );
			int lprintdec(char* ,const int & );
			int lprintflt(char* ,const double & );
			int lprintstr(char* ,const char* );
		public :
			LOGSPAWNER(const char* FileName = ".log" ,int Size = 4097 );
			~LOGSPAWNER(void );
			void clear(void );
			int lprint(const char* ,...);
			FILE* Redirect(const char* );
	};
}
#endif