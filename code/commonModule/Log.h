#ifndef _LOG_H_
#define _LOG_H_

#include <string>
#include <vector>
using namespace std;
//实际使用的Level
extern int  LogLevel[5];

//宏定义
#define MAX_STRING_LEN 10240

//日志类别
#define PRINT_LOG_LEVEL		0
#define DEBUG_LEVEL			1
#define INFO_LEVEL			2
#define WARNING_LEVEL		3
#define ERROR_LEVEL			4

#define TMIE_TYPE_YMD_LINE							"y-m-d"
#define TMIE_TYPE_YMD_NOSPACE						"ymd"
#define TMIE_TYPE_YMDHMS_POIT						"y.m.d h:m:s"
#define TMIE_TYPE_HMS_POIT							"h:m:s"
#define TIME_TYPE_HMS_NOSPACE                       "hms"

enum
{
	LOG_DT_TYPE_ALL = 0,
	LOG_DT_FIFO = 1,
	LOG_DT_CHR = 2,
	LOG_DT_DIR = 4,
	LOG_DT_BLK = 6,
	LOG_DT_REG = 8,
	LOG_DT_LNK = 10,
	LOG_DT_SOCK = 12,
	LOG_DT_WHT = 14
};

#ifdef __cplusplus  
extern "C" {               // 告诉编译器下列代码要以C链接约定的模式进行链接  
#endif  

//*****************************************************************
//函数名：LogInit
//功能  :初始化日志
//参数1 ：类型char*   日志的保存路径
//参数2 ：类型int     写日志的级别，小于设置级别不会写日志
//参数3 ：类型int     一个文件日志打印的最大行数
//参数4 ：类型int     日志保存的时间
//
//返回值 int
//		 0 成功 非0失败
//********************************************************************
int  LogInit(const char *strLogPath,int level,int MaxLogFileSize,int saveDays);

//*************************************************************
//函数名：WriteLog
//功能  : 将字符串输出到屏幕，根据级别显示不同颜色
//参数1 ：类型int     日志级别
//							 PRINT_LOG_LEVEL		0
//							 DEBUG_LEVEL			1
//							 INFO_LEVEL				2
//							 WARNING_LEVEL			3
//							 ERROR_LEVEL			4
//参数2 ：类型char*   字符串的内容
//返回值 无
//**************************************************************
void WriteLogTOScreen(int nlogLevel, const char *fmt, ...);

//*************************************************************
//函数名：WriteLog
//功能  :  
//参数1 ：类型int	  日志发生的文件名  __FILE__
//参数2 ：类型char*   日志发生的行号	__LINE__
//参数3 ：类型int     日志发生的函数名  __FUNC___
//参数4 ：类型int     日志级别
//							 PRINT_LOG_LEVEL		0
//							 DEBUG_LEVEL			1
//							 INFO_LEVEL				2
//							 WARNING_LEVEL			3
//							 ERROR_LEVEL			4
//参数5 ：类型char*   日志文件的名字为该参数的值
//参数6 ：类型char*   格式化后的日志内容
//
//返回值 无
//**************************************************************
void WriteToLog(const char* writeFile,int line,const char* funName,int level, const char *file, const char *fmt, ...);


//*************************************************************
//函数名：SetWriteLogLevel
//功能  : 设置写日志的级别，小于这个级别不会写日志
//参数1 ：类型 int 写日志的级别
//			PRINT_LOG_LEVEL			0
//			DEBUG_LEVEL				1
//			INFO_LEVEL				2
//			WARNING_LEVEL			3
//			ERROR_LEVEL				4
//
//返回值 无
//**************************************************************
void SetWriteLogLevel(int level);

//**************************************************************
//函数名：SetLogMaxLinage
//功能  :设置写一个日志文件的大小，文件超过设置的值会分页
//
//参数1 ：一个日志文件的最大大小
//
//返回值 无
//**************************************************************
void SetnMaxLogFileSize(int MaxLogFileSize);

//**************************************************************
//函数名：SetLogSavePath
//功能  :设置日志文件的保存路径
//参数1 ：类型char*   日志写入的路径
//
//返回值 无
//**************************************************************
void SetLogSavePath(const char*strLogPath);


//**************************************************************
//函数名：SetLogMaxLinage
//功能  :设置日志文件的保存时间，保存设置天数之前的日志将会被删除
//     
//参数1 ：类型int   日志保存的天数
//
//返回值 无
//**************************************************************
void SetLogSaveDays(int saveDays);

int Sprintf(char* destStr,const char* fmt, ...);

void GetLogSavePath(string& logSavePath);

void GetPathDir(string strCateDirPath,vector<string>&vecFile,int nFileType=LOG_DT_TYPE_ALL);

void split(const string& src, const string& separator, vector<string>& dest);

void MutexLock(void* mutex);

void MutexUnLock(void* mutex);

void logSleep(unsigned int useconds);

void ThreadDetach();

#ifdef __cplusplus  
}
#endif

#endif
