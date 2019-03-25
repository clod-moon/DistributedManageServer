#include "CommonTimer.h"
#include <string.h>
#include <stdlib.h>


CCommonTimer::CCommonTimer()
{
}


CCommonTimer::~CCommonTimer()
{
}

void CCommonTimer::CommonSleepMs(unsigned int msec)
{
#ifdef WIN32
	Sleep(msec);
#else
	usleep(msec*1000);
#endif
}

void CCommonTimer::CommonSleep(unsigned int sec)
{
#ifdef WIN32
	Sleep(sec*1000);
#else
	sleep(sec);
#endif
}


//日志文件中的时间
int  CCommonTimer::GetCurTime(char* strTime, char* szType)
{
	struct tm*		  tmTime = NULL;
	size_t			    timeLen = 0;
	time_t			    tTime = 0;
	tTime = time(NULL);
	tmTime = localtime(&tTime);
	if (0 == strcmp(szType, TMIE_TYPE_HMS_POIT))
	{
		timeLen = strftime(strTime, 33, "%H:%M:%S", tmTime);
	}
	else if (0 == strcmp(szType, TMIE_TYPE_YMDHMS_POIT))
	{
		timeLen = strftime(strTime, 33, "%Y.%m.%d %H:%M:%S", tmTime);
	}
	else if (0 == strcmp(szType, TMIE_TYPE_YMD_NOSPACE))
	{
		timeLen = strftime(strTime, 33, "%Y%m%d", tmTime);
	}
	else if (0 == strcmp(szType, TMIE_TYPE_YMD_LINE))
	{
		timeLen = strftime(strTime, 33, "%Y-%m-%d", tmTime);
	}
	else if (0 == strcmp(szType, TIME_TYPE_HMS_NOSPACE))
	{
		timeLen = strftime(strTime, 33, "%H%M%S", tmTime);
	}
	return timeLen;
}