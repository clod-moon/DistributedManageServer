/*=========================================================
 *文件名称:  Sock.h
 *创建日期：2013-8-29
 *修改记录：
 *  2013-8-29  created
===========================================================*/
#ifndef __CTcpSocket_H
#define __CTcpSocket_H

#ifdef WIN32

#else

#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#endif // WIN32




#define TIME_INFINITE -1
#define _TRUE_    1
#define _FALSE_   0
  

class CTcpSocket
{
public:
  CTcpSocket();
  CTcpSocket(const CTcpSocket& other);
  CTcpSocket(int iSock);
  ~CTcpSocket();
    
  CTcpSocket& operator=(const CTcpSocket& other);
  bool operator==(const CTcpSocket& other);

  int Connect(const char *szIp, int iPort);
  int Bind(const char *szIp, int iPort);
  int Listen(int iNum = 3);
  int Accept(CTcpSocket &conSock , int iTimeOut = 10);
  int Close();
  int Send(const char *szData, int iDataLen, int iTimeOut = 10);
  int Recv(char *szData, int iDataLen, int iTimeOut = 10);
  int GetHandle(); 

private:
  int Attach(int iSock);
  int IsReadyToRead(int iTimeOut);
  int IsReadyToWrite(int iTimeOut);
  
private:
  int m_iSock;
};

#endif