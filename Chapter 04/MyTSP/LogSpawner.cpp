#include "LogSpawner.h"
#pragma warning( disable:4996)
namespace RedContritio
{
	StringHolder::StringHolder(const char* str ):_str(str )
	{
		_len = strlen(_str );
	}
	StringHolder::StringHolder(const char* str ,int len ):_str(str ),_len(len )
	{
		;
	}

	LogSpawner::LogSpawner(const char* FileName ,int Size ):fout(NULL ),SizeOfBuffer(Size ),buflen(0 ),counter(0 )
	{
		if(FileName )fout = fopen(FileName ,"a+" );
		buffer = (char*)calloc(SizeOfBuffer ,sizeof(char ));
	}

	LogSpawner::~LogSpawner(void )
	{
		if(buffer )
		{
			buffer[buflen++] = '\n' ;
			clear( );
			free(buffer );
			buffer = NULL ;
		}
		if(fout )
		{
			fclose(fout );
			fout = NULL ;
		}		
	}

	#ifndef __LOGSPAWNER__OPTIMIZATION
	int LogSpawner::lprint(const char* Message )
	{
		char* temp = (char*)calloc(SizeOfBuffer ,sizeof(char ));
		int tmplen = 0 ;
		
		time_t rawtime;
		tm *timeinfo;

		time(&rawtime );
		timeinfo =localtime(&rawtime );
		
		sputs(temp+tmplen ,asctime(timeinfo ));
		tmplen = 20 ; temp[20] = 0 ;
		
		tmplen += sputs(temp+tmplen ,Message );
		tmplen += sputs(temp+tmplen ,"\n" );
		// assert(tmplen < SizeOfBuffer );
		load(temp );
		free(temp );
		return tmplen ;
	}
	
	void LogSpawner::load(const char* str )
	{
		int nlen = strlen(str );
		if(nlen +buflen >= SizeOfBuffer )
		{
			print( );
		}
		memcpy(buffer+buflen ,str ,nlen );
		buflen += nlen ;
		counter ++ ;
	}

	int LogSpawner::sputs(char* dest ,const char* source )
	{
		int len = strlen(source );
		memcpy(dest ,source ,len * sizeof(char ));
		return len ;
	}
	#else
	int LogSpawner::lprint(const char* Message )
	{
		char* temp = (char*)calloc(SizeOfBuffer ,sizeof(char ));
		int tmplen = 0 ;
		
		StringHolder MessageHolder(Message );
		time_t rawtime;
		tm *timeinfo;

		time(&rawtime );
		timeinfo =localtime(&rawtime );
		
		StringHolder TimeHolder(asctime(timeinfo ),20 );
		
		tmplen += sputs(temp+tmplen ,TimeHolder );
		tmplen += sputs(temp+tmplen ,MessageHolder );
		tmplen += sputs(temp+tmplen ,"\n" );
		// assert(tmplen < SizeOfBuffer );
		load(temp );
		free(temp );
		return tmplen ;
	}
	
	void LogSpawner::load(const StringHolder &StrHolder )
	{
		if(StrHolder._len +buflen >= SizeOfBuffer )
		{
			print( );
		}
		memcpy(buffer+buflen ,StrHolder._str ,StrHolder._len );
		buflen += StrHolder._len ;
		counter ++ ;
	}

	int LogSpawner::sputs(char* dest ,const StringHolder &StrHolder )
	{
		memcpy(dest ,StrHolder._str ,StrHolder._len * sizeof(char ));
		return StrHolder._len ;
	}
	#endif
	FILE* LogSpawner::Redirect(const char* FileName = ".log" )
	{
		if(fout )
		{
			clear( );
			fclose(fout );
		}
		fout = fopen(FileName ,"a+" );
		return fout ;
	}

	void LogSpawner::print(void )
	{
		fwrite(buffer ,buflen ,sizeof(char ),fout );
		memset(buffer ,0 ,buflen * sizeof(char ));
		buflen = 0 ;
	}

	

	void LogSpawner::clear(void )
	{
		if(buffer )
		{
			if(buflen )
			{
				print( );
			}
		}
	}

	
}