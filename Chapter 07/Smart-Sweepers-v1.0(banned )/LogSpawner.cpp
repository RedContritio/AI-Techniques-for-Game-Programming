#include "LogSpawner.h"
namespace RedContritio
{
	LogSpawner::LogSpawner(void ):fout(NULL ),SizeOfBuffer(4097 ),buflen(0 ),counter(0 ),isTimeStampOn(true )
	{
		fopen_s(&fout ,".log" ,"a+" );
		while(buffer == NULL )buffer = (char*)calloc(SizeOfBuffer ,sizeof(char ));
	}

	LogSpawner::LogSpawner(int Size ):fout(NULL ),SizeOfBuffer(Size ),buflen(0 ),counter(0 ),isTimeStampOn(true )
	{
		fopen_s(&fout ,".log" ,"a+" );
		while(buffer == NULL )buffer = (char*)calloc(SizeOfBuffer ,sizeof(char ));
	}

	LogSpawner::LogSpawner(const char* FileName ):fout(NULL ),SizeOfBuffer(4097 ),buflen(0 ),counter(0 ),isTimeStampOn(true )
	{
		if(FileName )fopen_s(&fout ,FileName ,"a+" );
		while(buffer == NULL )buffer = (char*)calloc(SizeOfBuffer ,sizeof(char ));
	}

	LogSpawner::LogSpawner(const char* FileName ,int Size ):fout(NULL ),SizeOfBuffer(Size ),buflen(0 ),counter(0 ),isTimeStampOn(true )
	{
		if(FileName )fopen_s(&fout ,FileName ,"a+" );
		while(buffer == NULL )buffer = (char*)calloc(SizeOfBuffer ,sizeof(char ));
	}

	LogSpawner::~LogSpawner(void )
	{
		if(buffer )
		{
			buffer[buflen++] = '\n' ;
			clear();
			free(buffer );
		}
		if(fout )
		{
			fclose(fout );
		}		
	}

	void LogSpawner::printTimeStamp(void )
	{
		if(isTimeStampOn )
		{
			char temp[64] = "" ;
			time_t rawtime;
			tm timemessage ;
			tm *timeinfo = &timemessage ;

			time(&rawtime );
			localtime_s(timeinfo ,&rawtime );
			asctime_s(temp ,timeinfo );

			// temp[20] = 0 ;
			
			load(temp ,20 );
		}
	}

	int __cdecl LogSpawner::printf(const char* Message ,...)
	{
		printTimeStamp() ;

		int BufferSize = 2*SizeOfBuffer ;
		char* temp = (char*)calloc(SizeOfBuffer ,BufferSize * sizeof(char ));
		int tmplen = 0 ;
		
		{
			double vargflt = 0.0 ;
			int vargint = 0 ;
			char* vargstr = NULL ;
			char vargch = 0 ;
			
			const char* pfmt = Message ;
			va_list vp ;
			va_start(vp ,Message );

			while(*pfmt )
			{
				if(*pfmt == '%' )
				{
					pfmt ++ ;
					switch(*pfmt )
					{
						case 'c' :
						{
							vargch = va_arg(vp ,int );
							tmplen += printch(temp+tmplen ,vargch );
							break ;
						}
						case 'i' : case 'd' :
						{
							vargint = va_arg(vp ,int );
							tmplen += printdec(temp+tmplen ,vargint );
							break ;
						}
						case 's' :
						{
							vargstr = va_arg(vp ,char* );
							tmplen += _printstr(temp+tmplen ,vargstr );
							break ;
						}
						case 'l' :
						{
							if(*(pfmt+1 ) != 'f' )
							{
								break ;
							}
							pfmt ++ ;
						}
						case 'f' :
						{
							vargflt = va_arg(vp ,double );
							tmplen += printflt(temp+tmplen ,vargflt );
							break ;
						}
						case '%' :
						{
							tmplen += printch(temp+tmplen ,'%' );
							break ;
						}
					}
				}
				else if(*pfmt == '\\' )
				{
					pfmt ++ ;
					switch(*pfmt )
					{
						case 'n' :
						{
							tmplen += printch(temp+tmplen ,'\n' );
							break ;
						}
						case '\\' :
						{
							tmplen += printch(temp+tmplen ,'\\' );
							break ;
						}
						case 't' :
						{
							tmplen += printch(temp+tmplen ,'\t' );
							break ;
						}
						case 'r' :
						{
							tmplen += printch(temp+tmplen ,'\t' );
							break ;
						}
						default :
							break ;
					}
				}
				else
				{
					tmplen += printch(temp+tmplen ,*pfmt );
				}
				pfmt ++ ;
			}
			va_end(vp );
			
		}

		load(temp );
		free(temp );
		return tmplen ;
	}
	
	int LogSpawner::puts(const char* str )
	{
		printTimeStamp() ;

		size_t len = strlen(str );
		char* nstr = (char* )malloc((len +1 +1 )*sizeof(char ));
		memcpy(nstr ,str ,len*sizeof(char ));
		nstr[len] = 0x0A ;
		nstr[len+1] = 0x00 ;
		load(nstr ,len+1 );
		return len+1 ;
	}

	int LogSpawner::putchar(int _ch )
	{
		buffer[buflen++] = _ch ;
		return 1 ;
	}

	void LogSpawner::load(const char* str )
	{
		load(str ,strlen(str ));
	}

	void LogSpawner::load(const char* str ,size_t len )
	{
		if(len +buflen >= SizeOfBuffer )
		{
			print();
		}
		memcpy(buffer+buflen ,str ,len );
		buflen += len ;
		counter ++ ;
	}

	int LogSpawner::sputs(char* dest ,const char* source )
	{
		int len = strlen(source );
		memcpy(dest ,source ,len * sizeof(char ));
		return len ;
	}

	FILE* LogSpawner::Redirect(const char* FileName = ".log" )
	{
		if(fout )
		{
			clear();
			fclose(fout );
		}
		fopen_s(&fout ,FileName ,"a+" );
		return fout ;
	}

	FILE* LogSpawner::Redirect(FILE* file )
	{
		if(file == NULL || ((unsigned long long)file & 1 ))return NULL ;
		if(fout )
		{
			clear();
			fclose(fout );
		}
		fout = file ;
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

	int LogSpawner::printch(char* dst ,const char &ch )
	{
		*dst = ch ;
		return 1 ;
	}
	int LogSpawner::printdec(char* dst ,const int &num )
	{
		char temp[36] = "" ;
		int len = sprintf_s(temp ,"%d" ,num );
		memcpy(dst ,temp ,len*sizeof(char) );
		return len ;
	}
	int LogSpawner::_printstr(char* dst ,const char* src )
	{
		size_t len = strlen(src );
		memcpy(dst ,src ,len*sizeof(src[0] ));
		return len*sizeof(src[0] );
	}
	int LogSpawner::printflt(char* dst ,const double &flt )
	{
		char temp[36] = "" ;
		int len = sprintf_s(temp ,"%g" ,flt );
		memcpy(dst ,temp ,len*sizeof(char) );
		return len ;
	}

	
}