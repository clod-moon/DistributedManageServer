#include <iostream>
#include "Log.h"
#include "GetConfig.h"
//#include "../include/commonModule/GetConfig.h"
//#include "../include/commonModule/Log.h"
using namespace std;

map<string, string> mapConfig;

int main()
{

	if (!ReadConfig("./config.ini", mapConfig))
	{
		WriteLogTOScreen(ERROR_LEVEL,"�������ļ�����");
	}
	return 0;
}