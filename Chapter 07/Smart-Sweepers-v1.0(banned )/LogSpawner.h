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
	class LogSpawner
	{
		private :
			FILE* fout ;
			size_t SizeOfBuffer ;
			char* buffer ;
			int buflen ;
			bool isTimeStampOn ;
			mutable int counter ;
			void print(void );
			void load(const char* );
			void load(const char* ,size_t );

			int sputs(char* ,const char* );
			void printTimeStamp(void );
			int printch(char* ,const char & );
			int printdec(char* ,const int & );
			int printflt(char* ,const double & );
			int _printstr(char* ,const char* );
		public :
			LogSpawner(void );
			LogSpawner(int );
			LogSpawner(const char* );
			LogSpawner(const char* ,int );
			~LogSpawner(void );
			void clear(void );
			int __cdecl printf(const char* format ,...);
			int puts(const char* str );
			int putchar(int _ch );
			bool ToggleTimeStamp(void ){return (isTimeStampOn = !isTimeStampOn );}
			FILE* Redirect(const char* );
			FILE* Redirect(FILE* );
	};
}
#endif