#ifndef __LOG__SPAWN__INC
#define __LOG__SPAWN__INC
#include <cstdarg>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cassert>
#include <ctime>

//#define __LOGSPAWNER__OPTIMIZATION
 
namespace RedContritio
{
	struct StringHolder
	{
		const char* _str ;
		int _len ;
		StringHolder(const char* str );
		StringHolder(const char* str ,int len );
	};

	class LogSpawner
	{
		private :
			FILE* fout ;
			int SizeOfBuffer ;
			char* buffer ;
			int buflen ;
			mutable int counter ;
			void print(void );
			#ifndef __LOGSPAWNER__OPTIMIZATION
			void load(const char* );

			int sputs(char* ,const char* );
			#else
			void load(const StringHolder & );

			int sputs(char* ,const StringHolder & );
			#endif
			int lprintch(char* ,const char & );
			int lprintdec(char* ,const int & );
			int lprintflt(char* ,const double & );
			int lprintstr(char* ,const char* );
		public :
			LogSpawner(const char* FileName = ".log" ,int Size = 4097 );
			~LogSpawner(void );
			void clear(void );
			int lprint(const char* ,...);
			FILE* Redirect(const char* );
	};
}
#endif