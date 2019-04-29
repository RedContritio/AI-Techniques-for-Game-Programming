#include "LogSpawner.h"
#include <cstdio>
#include <cstdarg>

#pragma warning(disable : 4996)
namespace RedContritio
{

LogSpawner::LogSpawner(const char *_FileName) : m_fout(NULL), m_toclose(false), m_timestamp(true)
{
	if ( _FileName ) this->open(_FileName);
}

LogSpawner::LogSpawner(FILE *_Stream) : m_fout(_Stream), m_toclose(false), m_timestamp(true){}

LogSpawner::~LogSpawner(void)
{
	if ( this->m_fout ) this->close();
}

void LogSpawner::open(const char *_FileName)
{
	assert(_FileName); // FileName != NULL
#ifdef __STDC_WANT_SECURE_LIB__
	fopen_s(&(this->m_fout), _FileName, "a+");
#else
	this->m_fout = fopen(_FileName, "a+");
#endif
	if ( !(this->m_fout) ) throw("File open failed.");
	this->m_toclose = true;
}

void LogSpawner::close(void)
{
	assert(this->m_fout);
	fclose(this->m_fout);
	this->m_fout = NULL;
	this->m_toclose = false;
}

int LogSpawner::printf(const char *_Format, ...) const
{
	if ( !(this->m_fout) ) return 0;

	if ( m_timestamp )timestamp(this->m_fout);

	va_list _ArgList;
	va_start(_ArgList, _Format);
	int n = vfprintf(this->m_fout, _Format, _ArgList);
	va_end(_ArgList);

	return n;
}

void LogSpawner::SetTimeStamp(bool state)
{
	m_timestamp = state;
}

FILE *LogSpawner::redirect(const char *_FileName)
{
	if ( this->m_fout && this->m_toclose) this->close();
	if ( _FileName ) this->open(_FileName);
	return this->m_fout;
}

FILE *LogSpawner::redirect(FILE *_Stream)
{
	if ( this->m_fout && this->m_toclose ) this->close();
	this->m_fout = _Stream;
	return this->m_fout;
}

void LogSpawner::timestamp(FILE *_Stream)
{
	time_t rawtime;
	tm *timeinfo;
	time(&rawtime);
	timeinfo = localtime(&rawtime);
	fprintf(_Stream, "%d:%d:%d ", timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);
}

}