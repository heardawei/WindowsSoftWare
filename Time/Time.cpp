
#include "stdafx.h"
#include <time.h>

#ifdef _WIN32
#include <windows.h>
int gettimeofday(struct timeval *tv, void *tz);
#else
#  include <sys/time.h>
#endif

#ifdef _WIN32
int gettimeofday(struct timeval *tv, void *tz)
{
	time_t clock;
	struct tm tm;
	SYSTEMTIME wtm;
	GetLocalTime(&wtm);
	tm.tm_year = wtm.wYear - 1900;
	tm.tm_mon = wtm.wMonth - 1;
	tm.tm_mday = wtm.wDay;
	tm.tm_hour = wtm.wHour;
	tm.tm_min = wtm.wMinute;
	tm.tm_sec = wtm.wSecond;
	tm.tm_isdst = -1;
	clock = mktime(&tm);
	tv->tv_sec = clock;
	tv->tv_usec = wtm.wMilliseconds * 1000;
	return (0);
}
#endif

#include <iostream>
#include <sys/utime.h>
int main(void)
{
	struct timeval tv;
	gettimeofday(&tv, NULL);
	std::cout << tv.tv_sec << "." << tv.tv_usec << std::endl;
	getchar();
	return 0;
}