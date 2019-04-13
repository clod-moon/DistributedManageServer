#ifndef _SOCKETCOMM_H_
#define _SOCKETCOMM_H_

#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <memory>
#include <errno.h>
#include <string>

typedef  struct sockaddr_in   SockAddr;

//AF_INET ���Ǵ������������socket��Э�飬ʹ��TCP��UDP�����䣬��IPv4�ĵ�ַ
//AF_INET6 ���������ƣ�����������IPv6�ĵ�ַ
//AF_UNIX ����Э�飬ʹ����Unix��Linuxϵͳ�ϣ�һ�㶼�ǵ��ͻ��˺ͷ�������ͬһ̨�����ϵ�ʱ��ʹ��
//type :
//SOCK_STREAM ���Э���ǰ���˳��ġ��ɿ��ġ����������Ļ����ֽ��������ӡ�
//����һ��ʹ������socket���ͣ����socket��ʹ��TCP�����д��䡣
//SOCK_DGRAM ���Э���������ӵġ��̶����ȵĴ�����á���Э���ǲ��ɿ��ģ�ʹ��UDP�������������ӡ�
//SOCK_SEQPACKET��Э����˫��·�ġ��ɿ������ӣ����͹̶����ȵ����ݰ����д��䡣���������������Ľ��ܲ��ܽ��ж�ȡ��
//SOCK_RAW socket�����ṩ��һ��������ʣ����socket����ʹ��ICMP����Э�顣��ping��tracerouteʹ�ø�Э�飩
//SOCK_RDM ��������Ǻ���ʹ�õģ��ڴ󲿷ֵĲ���ϵͳ��û��ʵ�֣������ṩ��������·��ʹ�ã�����֤���ݰ���˳��
//protocol :
//��0 ��ʾʹ��Ĭ��Э�顣
//�ɹ�������ָ���´�����socket���ļ���������ʧ�ܣ�����-1������errno

int CreateSocket(int domin, int type, int protocol)
{
	return socket(domin, type, protocol);
}

SockAddr* CreateSockaddr(int nPort)
{
	//std::shared_ptr<struct sockaddr_in> pSockaddr(new struct sockaddr_in);
	SockAddr* pSockAddr = new SockAddr;
	pSockAddr->sin_family = AF_INET;
	//addr.sin_addr.s_addr = inet_addr(lpIp);
	pSockAddr->sin_addr.s_addr = htonl(INADDR_ANY);
	pSockAddr->sin_port = htons(nPort);
	return pSockAddr;
}

void DestroySockaddr(SockAddr* pSockAddr)
{
	//std::shared_ptr<struct sockaddr_in> pSockaddr(new struct sockaddr_in);
	delete pSockAddr;
	return;
}

/*
sockfd��socket�ļ�������
addr :�����IP��ַ�Ӷ˿ں�
addrlen :sizeof(addr)����
����ֵ��
�ɹ�����0��ʧ�ܷ��� - 1, ����errno
*/
int Bind(int sockfd, SockAddr* pSockaddr)
{
	int iRet;

	iRet = bind(sockfd, (sockaddr*)pSockaddr, sizeof(*pSockaddr));
	if (iRet < 0)
	{
		return iRet;
	}
	return iRet;
}
/*
����˵����
sockfd �����׽���
backlog ����������ӵĿͻ�������
����ֵ��
0 �ɹ�
-1 ʧ��
*/
int Listen(int sockfd, int backlog)
{
	return listen(sockfd, backlog);
}

/*
sockdf: socket�ļ�������
addr :�����������������ӿͻ��˵�ַ��Ϣ����IP��ַ�Ͷ˿ں�
addrlen :���봫��������ֵ - �����, ����sizeof(addr)��С��
��������ʱ�����������յ���ַ�ṹ��Ĵ�С
����ֵ��
�ɹ� ����һ���µ�socket�ļ������������ںͿͻ���ͨ��
ʧ�ܷ��� - 1������errno
*/
int Accept(int sockfd, SockAddr *addr)
{
	int iSock;
	socklen_t iLen;

	iSock = accept(sockfd, (sockaddr*)addr, &iLen);
	if (0 > iSock)
	{
		return -1;
	}
	return iSock;
}


int Connect(std::string strIp, int nPort)
{
	struct sockaddr_in  addr;
	int iRet;

	int nSock = socket(AF_INET, SOCK_STREAM, 0);
	if (nSock < 0)
	{
		return -1;
	}

	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = inet_addr(strIp.c_str());
	addr.sin_port = htons(nPort);

	iRet = connect(nSock, (struct sockaddr*)&addr, sizeof(addr));
	if (iRet < 0)
	{
		return iRet;
	}
	return iRet;
}

int Send(int fd, const char *lpData, int iDataLen)
{
	int iRet, iCounter, iWantLen;

	iCounter = 0;
	iWantLen = iDataLen;

	while (iCounter != iDataLen)
	{

		iRet = write(fd, lpData + iCounter, iWantLen);
		//printf("lpData=%s\n", lpData);
		if (iRet == -1)
		{
			if (errno == EPIPE)
				return 0;
			else
				return -1;
		}
		iCounter += iRet;
		iWantLen = iDataLen - iCounter;

	}
	return (iCounter);
}

int Recv(int fd, char *lpData, int iDataLen)
{
	int iRet, iCounter, iWantLen;

	iCounter = 0;
	iWantLen = iDataLen;

	while (iCounter != iDataLen)
	{
		iRet = read(fd, lpData + iCounter, iWantLen);
		//printf("lpData=%s\n", lpData);
		if (iRet <= 0)
		{
			return iRet;
		}
		if (iRet < iWantLen)
		{
			return iCounter+iRet;
		}
		iCounter += iRet;
		iWantLen = iDataLen - iCounter;

	}
	return (iCounter);
}


#endif
