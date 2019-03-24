//*****************************************************************************
//说明：	将常用的一些对时间操作的方法封装成了类
//
//作者：	冷月莫负
//创建时间：2018.01.19
//
//*****************************************************************************
#ifndef __COMMON_TIMER__H_
#define __COMMON_TIMER__H_
#include <time.h>
#include <sys/timeb.h>

#ifdef WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

#define TMIE_TYPE_YMD_LINE							"y-m-d"
#define TMIE_TYPE_YMD_NOSPACE						"ymd"
#define TMIE_TYPE_YMDHMS_POIT						"y.m.d h:m:s"
#define TMIE_TYPE_HMS_POIT							"h:m:s"
#define TIME_TYPE_HMS_NOSPACE                       "hms"

class CCommonTimer
{
public:
	CCommonTimer();
	~CCommonTimer();
public:
	//****************************************************************************
	//函数：CommonSleepMs
	//功能：让当前进程或者线程暂停传入的毫秒时间
	//参数：1 类型 unsigned int 
	//			希望程序延时的毫秒数
	//返回值：类型void  
	//
	//*****************************************************************************
	void CommonSleepMs(unsigned int msec);

	//****************************************************************************
	//函数：CommonSleep
	//功能：让当前进程或者线程暂停传
	//参数：1 类型 unsigned int 
	//			希望程序延时的秒数
	//返回值：类型int  希望程序延时的时间长度
	//
	//*****************************************************************************
	void CommonSleep(unsigned int sec);

	//****************************************************************************
	//函数：GetCurTime
	//功能：获取当前系统时间，公元计时时间
	//参数：1 类型char* 
	//			获取到的系统时间		
	//		2 类型char*
	//			要获取的时间格式
	//          TMIE_TYPE_YMD_LINE							"y-m-d"
	//			TMIE_TYPE_YMD_NOSPACE						"ymd"
	//			TMIE_TYPE_YMDHMS_POIT						"y.m.d h:m:s"
	//			TMIE_TYPE_HMS_POIT							"h:m:s"
	//			TIME_TYPE_HMS_NOSPACE                       "hms"
	//
	//返回值：类型void  
	//*****************************************************************************
	int  GetCurTime(char* szCurTime, char* szType);

	//****************************************************************************
	//函数：GetCUTTime
	//功能：获取当前系统时间
	//参数：无
	//
	//返回值：类型long  返回1970-1-1, 00:00:00以来经过的毫秒数
	//
	//*****************************************************************************
	long GetCUTTime();
};

#endif
