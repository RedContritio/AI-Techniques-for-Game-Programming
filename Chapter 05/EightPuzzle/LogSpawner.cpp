#include "LogSpawner.h"
#pragma warning(disable : 4996)
namespace RedContritio
{
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

	int LogSpawner::lprint(const char* Message ,...)
	{
		int BufferSize = (SizeOfBuffer > (int)(8*strlen(Message)))?(SizeOfBuffer ):(8 *strlen(Message ));
		char* temp = (char*)calloc(SizeOfBuffer ,BufferSize * sizeof(char ));
		int tmplen = 0 ;
		
		time_t rawtime;
		tm *timeinfo = NULL ;

		time(&rawtime );
		timeinfo = localtime(&rawtime );
		
		sputs(temp+tmplen ,asctime(timeinfo ));
		tmplen = 20 ; temp[20] = 0 ;
		
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
							tmplen += lprintch(temp+tmplen ,vargch );
							break ;
						}
						case 'i' : case 'd' :
						{
							vargint = va_arg(vp ,int );
							tmplen += lprintdec(temp+tmplen ,vargint );
							break ;
						}
						case 's' :
						{
							vargstr = va_arg(vp ,char* );
							tmplen += lprintstr(temp+tmplen ,vargstr );
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
							tmplen += lprintflt(temp+tmplen ,vargflt );
							break ;
						}
						case '%' :
						{
							tmplen += lprintch(temp+tmplen ,'%' );
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
							tmplen += lprintch(temp+tmplen ,'\n' );
							break ;
						}
						case '\\' :
						{
							tmplen += lprintch(temp+tmplen ,'\\' );
							break ;
						}
						case 't' :
						{
							tmplen += lprintch(temp+tmplen ,'\t' );
							break ;
						}
						case 'r' :
						{
							tmplen += lprintch(temp+tmplen ,'\t' );
							break ;
						}
						default :
							break ;
					}
				}
				else
				{
					tmplen += lprintch(temp+tmplen ,*pfmt );
				}
				pfmt ++ ;
			}
			va_end(vp );
			
		}

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

	int LogSpawner::lprintch(char* dst ,const char &ch )
	{
		*dst = ch ;
		return 1 ;
	}
	int LogSpawner::lprintdec(char* dst ,const int &num )
	{
		char* temp = (char*)calloc(12 ,sizeof(char));
		sprintf(temp ,"%d" ,num );
		int len = strlen(temp );
		memcpy(dst ,temp ,len*sizeof(char) );
		free(temp );
		return len ;
	}
	int LogSpawner::lprintstr(char* dst ,const char* src )
	{
		int len = strlen(src );
		memcpy(dst ,src ,len*sizeof(char) );
		return len ;
	}
	int LogSpawner::lprintflt(char* dst ,const double &flt )
	{
		char* temp = (char*)calloc(36 ,sizeof(char));
		sprintf(temp ,"%lf" ,flt );
		int len = strlen(temp );
		memcpy(dst ,temp ,len*sizeof(char) );
		free(temp );
		return len ;
	}

	
}