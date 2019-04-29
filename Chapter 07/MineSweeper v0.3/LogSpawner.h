#ifndef __LOG_SPAWN__INC
#define __LOG_SPAWN__INC
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
			FILE* m_fout ;
			bool m_toclose;
			bool m_timestamp;
			void open(const char* FileName);
			void close(void);
			static void timestamp(FILE *_Stream);
		public :
			LogSpawner(const char* FileName = ".log");
			LogSpawner(FILE *_Stream);
			~LogSpawner(void);
			int printf(const char* ,...) const;
			void SetTimeStamp(bool state);
			FILE* redirect(const char* );
			FILE *redirect(FILE *_Stream);
	};
}
#endif