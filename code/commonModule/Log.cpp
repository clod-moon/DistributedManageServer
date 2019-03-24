#ifdef WIN32
#define _CRT_SECURE_NO_WARNINGS
#endif
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <vector>
#include <fstream>
#include <stdlib.h>
#include <queue>
#include <list>
#include <map>
#include <algorithm>

#include "Log.h"

#ifdef WIN32
#include <Windows.h>
#include <direct.h>
#include <io.h>

#else
#include <unistd.h>
#include <dirent.h>
#include <pthread.h>
#endif

using namespace std;

//宏定义
#define MAX_STRING_LEN 10240

int			nLogDays = 1;
int			nWriteLogLevel = PRINT_LOG_LEVEL;
int			nMaxLogFileSize = 100;
int			nMaxSaveDays = 3;
string      strCurrentDate;


#ifdef WIN32
string  strLogPath = "D:";
int     mutex = 0;
#else
string  strLogPath = "./log";
#endif

int LogLevel[5] =
{
	PRINT_LOG_LEVEL,
	DEBUG_LEVEL,
	INFO_LEVEL,
	WARNING_LEVEL,
	ERROR_LEVEL
};

//日志的名称
char LevelName[5][10] =
{
	"PRINT",
	"DEBUG",
	"INFO",
	"WARNING",
	"ERROR"
};

void logSleep(unsigned int useconds)
{
#ifdef WIN32
	Sleep(useconds);
#else
	usleep(useconds * 1000);
#endif
}


bool  FileIsExist(string strLogPath)
{
#ifdef WIN32
	return true;
#else
	if (access(strLogPath.c_str(), F_OK) == 0)
		return true;
	else
		return false;
#endif
}

void Mkdir(string strLogPath)
{
#ifdef WIN32
#else
	//mkdir(strLogPath.c_str(),);
	if (!strLogPath.compare(".") || !strLogPath.compare(".."))
		return ;
	char szCmd[128] = { 0 };
	sprintf(szCmd, "mkdir %s", strLogPath.c_str());
	system(szCmd);
#endif
}

int Sprintf(char* destStr, const char* fmt, ...)
{
	int destStrLen;
	va_list args;
	va_start(args, fmt);
#ifdef WIN32
	destStrLen = vsprintf_s(destStr, MAX_STRING_LEN, fmt, args);
#else
	destStrLen = vsprintf(destStr, fmt, args);

#endif
	va_end(args);
	return destStrLen;
}

void split(const string& src, const string& separator, vector<string>& dest)
{
	string str = src;
	string substring;
	string::size_type start = 0, index;

	do
	{
		index = str.find_first_of(separator, start);
		if (index != string::npos)
		{
			substring = str.substr(start, index - start);
			dest.push_back(substring);
			start = str.find_first_not_of(separator, index);
			if (start == string::npos) return;
		}
	} while (index != string::npos);

	//the last token
	substring = str.substr(start);
	dest.push_back(substring);
}


//生成日志文件的时间
static int LogFileTime(char* fileTime)
{
	struct tm*		  tmTime = NULL;
	size_t			    timeLen = 0;
	time_t			    tTime = 0;
	tTime = time(NULL);
	tmTime = localtime(&tTime);
	timeLen = strftime(fileTime, 33, "%Y%m%d", tmTime);
	return timeLen;
}

//日志文件中的时间
static int GetCurTime(char* strTime, char* Type)
{
	struct tm*		  tmTime = NULL;
	size_t			    timeLen = 0;
	time_t			    tTime = 0;
	tTime = time(NULL);
	tmTime = localtime(&tTime);
	if (0 == strcmp(Type, TMIE_TYPE_HMS_POIT))
	{
		timeLen = strftime(strTime, 33, "%H:%M:%S", tmTime);
	}
	else if (0 == strcmp(Type, TMIE_TYPE_YMDHMS_POIT))
	{
		timeLen = strftime(strTime, 33, "%Y.%m.%d %H:%M:%S", tmTime);
	}
	else if (0 == strcmp(Type, TMIE_TYPE_YMD_NOSPACE))
	{
		timeLen = strftime(strTime, 33, "%Y%m%d", tmTime);
	}
	else if (0 == strcmp(Type, TMIE_TYPE_YMD_LINE))
	{
		timeLen = strftime(strTime, 33, "%Y-%m-%d", tmTime);
	}
	else if (0 == strcmp(Type, TIME_TYPE_HMS_NOSPACE))
	{
		timeLen = strftime(strTime, 33, "%H%M%S", tmTime);
	}
	return timeLen;
}


void WriteLogTOScreen(int nlogLevel, const char *fmt, ...)
{
	char logBody[MAX_STRING_LEN] = { 0 };
	va_list vaList;
	va_start(vaList, fmt);
	vsnprintf(logBody, MAX_STRING_LEN - 1, fmt, vaList);
	va_end(vaList);
	if (nlogLevel < WARNING_LEVEL)
	{
		printf("\x1b[0;34m%s\n", logBody);
	}
	else if (nlogLevel < ERROR_LEVEL)
	{
		printf("\x1b[0;33m%s\n", logBody);
	}
	else
	{
		printf("\x1b[0;31m%s\n", logBody);
	}
	return;
}

void WriteToLog(const char* writeFile, int line, const char* funName, int level, const char *file, const char *fmt, ...)
{
	char            str[MAX_STRING_LEN];
	int				strLen = 0;
	char            tmpStr[64];
	int				tmpStrLen = 0;
	char            szLogBody[MAX_STRING_LEN];

	memset(tmpStr, 0x00, sizeof(tmpStr));
	GetCurTime(tmpStr, TMIE_TYPE_YMD_LINE);
	if (strCurrentDate.compare(tmpStr) != 0)
	{
		char szCmd[64];
		strCurrentDate = tmpStr;
		vector<string>vecFile;
		GetPathDir(strLogPath, vecFile, LOG_DT_DIR);
		vector<string>::iterator itFile = vecFile.begin();
		while (vecFile.size() >= nMaxSaveDays)
		{
			Sprintf(szCmd, "rm -rf %s/%s", strLogPath.c_str(), itFile->c_str());
			system(szCmd);
			itFile = vecFile.erase(itFile);
		}
		memset(szCmd, 0x00, sizeof(szCmd));
		char szDirPath[64] = { 0 };
		Sprintf(szDirPath, "%s/%s", strLogPath.c_str(), strCurrentDate.c_str());
		if (!FileIsExist(szDirPath))
		{
			Sprintf(szCmd, "mkdir %s/%s", strLogPath.c_str(), strCurrentDate.c_str());
			system(szCmd);
		}
	}
	memset(str, 0, MAX_STRING_LEN);
	memset(tmpStr, 0, 64);

	//加入LOG时间
	tmpStrLen = GetCurTime(tmpStr, TMIE_TYPE_HMS_POIT);
	tmpStrLen = Sprintf(str, "[%s] ", tmpStr);
	strLen = tmpStrLen;

	//写日志文件
	tmpStrLen = Sprintf(str + strLen, "[%s ", writeFile);
	strLen += tmpStrLen;

	//日志所在行号
	tmpStrLen = Sprintf(str + strLen, "%d行] ", line);
	strLen += tmpStrLen;

	//日志所在函数
	tmpStrLen = Sprintf(str + strLen, "[%s] ", funName);
	strLen += tmpStrLen;

	//加入LOG等级
	tmpStrLen = Sprintf(str + strLen, "[%s] ", LevelName[level]);
	strLen += tmpStrLen;

	va_list args;
	va_start(args, fmt);
	vsnprintf(szLogBody, MAX_STRING_LEN - 1, fmt, args);
	va_end(args);

	//加入LOG信息
	tmpStrLen = Sprintf(str + strLen, "[%s]\n", szLogBody);
	strLen += tmpStrLen;

	WriteLogTOScreen(level,"%s",szLogBody);
	if (level < nWriteLogLevel)
	{
		return;
	}
	char szCmd[128] = { 0 };
	char szTmpLogName[128] = { 0 };
	Sprintf(szTmpLogName, "%s/%s/%s", strLogPath.c_str(), strCurrentDate.c_str(), writeFile);
	FILE* fileHandle = fopen(szTmpLogName, "a+");
	if (fileHandle != NULL)
	{
		fwrite(str, 1, strLen, fileHandle);
		fseek(fileHandle, 0, SEEK_END);
		if (ftell(fileHandle) / (1024 * 1024) >= nMaxLogFileSize)
		{
			fclose(fileHandle);
			char szTime[33] = { 0 };
			GetCurTime(szTime, TIME_TYPE_HMS_NOSPACE);
			Sprintf(szCmd, "mv %s %s_%s", szTmpLogName, szTmpLogName, szTime);
			int ret = system(szCmd);
			if (ret != -1 && ret != 127)
			{
				WriteLogTOScreen(ERROR_LEVEL,"%s",szCmd);
			}
		}
		else
		{
			fclose(fileHandle);
		}
	}
	else
	{
		WriteLogTOScreen(ERROR_LEVEL, "打开日志文件失败");
	}
	return;
}


int  LogInit(const char *szLogPath, int level, int MaxLogFileSize, int saveDays)
{
	SetLogSavePath(szLogPath);
	if (!FileIsExist(szLogPath))
	{
		string cmd;
		string tmpPath;
		vector<string>vPath;
		split(szLogPath, "/", vPath);
		for (int i = 0; i < vPath.size(); i++)
		{
			if ((i == 0) && (vPath[0].empty()))
			{
				tmpPath += "/";
			}
			if (i)
			{
				tmpPath += "/";
			}
			tmpPath += vPath[i];
			if (!FileIsExist(szLogPath))
			{
				Mkdir(tmpPath);
			}
		}
	}

	nWriteLogLevel = level;

	nMaxLogFileSize = MaxLogFileSize;

	nMaxSaveDays = saveDays;

	return 0;
}

void SetnMaxLogFileSize(int MaxLogFileSize)
{
	nMaxLogFileSize = MaxLogFileSize;
	return;
}

void SetLogSavePath(const char*szLogPath)
{
	if (strlen(szLogPath) > 0)
	{
		strLogPath = szLogPath;
		if (szLogPath[strlen(szLogPath) - 1] == '/')
		{
			strLogPath[strlen(szLogPath) - 1] = '/0';
		}
	}
}


void SetWriteLogLevel(int level)
{
	nWriteLogLevel = level;
}

void GetPathDir(string strCateDirPath, vector<string>&vecFile, int nFileType)
{
	
#ifdef WIN32
	_finddata_t file;
	long lf;
	//输入文件夹路径
	if ((lf = _findfirst(strCateDirPath.c_str(), &file)) != -1)
	{
		while (_findnext(lf, &file) == 0) {
			if (strcmp(file.name, ".") == 0 || strcmp(file.name, "..") == 0)
				continue;
			vecFile.push_back(file.name);
		}
	}
	_findclose(lf);
	
#else
	DIR *dir;
	struct dirent *ptr;
	char base[1000];

	if ((dir = opendir(strCateDirPath.c_str())) == NULL)
	{
		perror("Open dir error...");
		exit(1);
	}

	while ((ptr = readdir(dir)) != NULL)
	{
		if (strcmp(ptr->d_name, ".") == 0 || strcmp(ptr->d_name, "..") == 0 || ptr->d_type<nFileType)
			continue;
		else if (ptr->d_type == LOG_DT_REG)
			vecFile.push_back(ptr->d_name);
		else if (ptr->d_type == LOG_DT_LNK)
			continue;
		else if (ptr->d_type == LOG_DT_DIR)
		{
			char szcomd[128] = { 0 };
			vecFile.push_back(ptr->d_name);
		}
	}
	closedir(dir);
#endif

	//排序，按从小到大排序
	sort(vecFile.begin(), vecFile.end());
	return;
}
