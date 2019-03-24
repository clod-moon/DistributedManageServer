//*****************************************************************************
//˵����	�����õ�һЩ��ʱ������ķ�����װ������
//
//���ߣ�	����Ī��
//����ʱ�䣺2018.01.19
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
	//������CommonSleepMs
	//���ܣ��õ�ǰ���̻����߳���ͣ����ĺ���ʱ��
	//������1 ���� unsigned int 
	//			ϣ��������ʱ�ĺ�����
	//����ֵ������void  
	//
	//*****************************************************************************
	void CommonSleepMs(unsigned int msec);

	//****************************************************************************
	//������CommonSleep
	//���ܣ��õ�ǰ���̻����߳���ͣ��
	//������1 ���� unsigned int 
	//			ϣ��������ʱ������
	//����ֵ������int  ϣ��������ʱ��ʱ�䳤��
	//
	//*****************************************************************************
	void CommonSleep(unsigned int sec);

	//****************************************************************************
	//������GetCurTime
	//���ܣ���ȡ��ǰϵͳʱ�䣬��Ԫ��ʱʱ��
	//������1 ����char* 
	//			��ȡ����ϵͳʱ��		
	//		2 ����char*
	//			Ҫ��ȡ��ʱ���ʽ
	//          TMIE_TYPE_YMD_LINE							"y-m-d"
	//			TMIE_TYPE_YMD_NOSPACE						"ymd"
	//			TMIE_TYPE_YMDHMS_POIT						"y.m.d h:m:s"
	//			TMIE_TYPE_HMS_POIT							"h:m:s"
	//			TIME_TYPE_HMS_NOSPACE                       "hms"
	//
	//����ֵ������void  
	//*****************************************************************************
	int  GetCurTime(char* szCurTime, char* szType);

	//****************************************************************************
	//������GetCUTTime
	//���ܣ���ȡ��ǰϵͳʱ��
	//��������
	//
	//����ֵ������long  ����1970-1-1, 00:00:00���������ĺ�����
	//
	//*****************************************************************************
	long GetCUTTime();
};

#endif
