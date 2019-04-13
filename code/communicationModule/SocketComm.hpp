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

//AF_INET 这是大多数用来产生socket的协议，使用TCP或UDP来传输，用IPv4的地址
//AF_INET6 与上面类似，不过是来用IPv6的地址
//AF_UNIX 本地协议，使用在Unix和Linux系统上，一般都是当客户端和服务器在同一台及其上的时候使用
//type :
//SOCK_STREAM 这个协议是按照顺序的、可靠的、数据完整的基于字节流的连接。
//这是一个使用最多的socket类型，这个socket是使用TCP来进行传输。
//SOCK_DGRAM 这个协议是无连接的、固定长度的传输调用。该协议是不可靠的，使用UDP来进行它的连接。
//SOCK_SEQPACKET该协议是双线路的、可靠的连接，发送固定长度的数据包进行传输。必须把这个包完整的接受才能进行读取。
//SOCK_RAW socket类型提供单一的网络访问，这个socket类型使用ICMP公共协议。（ping、traceroute使用该协议）
//SOCK_RDM 这个类型是很少使用的，在大部分的操作系统上没有实现，它是提供给数据链路层使用，不保证数据包的顺序
//protocol :
//传0 表示使用默认协议。
//成功：返回指向新创建的socket的文件描述符，失败：返回-1，设置errno

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
sockfd：socket文件描述符
addr :构造出IP地址加端口号
addrlen :sizeof(addr)长度
返回值：
成功返回0，失败返回 - 1, 设置errno
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
参数说明：
sockfd 监听套接字
backlog 最多允许连接的客户端数量
返回值：
0 成功
-1 失败
*/
int Listen(int sockfd, int backlog)
{
	return listen(sockfd, backlog);
}

/*
sockdf: socket文件描述符
addr :传出参数，返回链接客户端地址信息，含IP地址和端口号
addrlen :传入传出参数（值 - 结果）, 传入sizeof(addr)大小，
函数返回时返回真正接收到地址结构体的大小
返回值：
成功 返回一个新的socket文件描述符，用于和客户端通信
失败返回 - 1，设置errno
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
