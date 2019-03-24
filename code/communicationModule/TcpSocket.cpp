/*=========================================================
 *文件名称:  Sock.cpp
 *创建日期： 2013-8-29
 *修改记录： 2017-2-23
 *修改者：   zyc
 *2013-8-29  created
===========================================================*/

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
//#include <sys/socket.h>
//#include <netinet/in.h>
//#include <netdb.h>
#include <fcntl.h>
#include <iostream>
#include "TcpSocket.h"
using namespace std;

CTcpSocket::CTcpSocket()
{
  m_iSock = 0;
}

CTcpSocket::CTcpSocket(const CTcpSocket& other)
{
	if (*this == other)
		return;
	if (m_iSock > 0)
	{
		Close();
	}
	m_iSock = other.m_iSock;
}

CTcpSocket::CTcpSocket(int iSock)
{
  m_iSock = iSock;
}

CTcpSocket::~CTcpSocket()
{
}
    
CTcpSocket& CTcpSocket::operator=(const CTcpSocket& other)
{
	return *this;
}

bool CTcpSocket::operator==(const CTcpSocket& other)
{
	return true;
}

int CTcpSocket::Connect(const char *lpIp, int iPort)
{
  struct sockaddr_in  addr;
  int iRet;
  
  m_iSock = socket( AF_INET, SOCK_STREAM, 0 ); 
  if(m_iSock < 0)
  {
    return -1;
  }
  
  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = inet_addr(lpIp);
  addr.sin_port = htons( iPort ); 
  
  iRet = connect( m_iSock,(struct sockaddr*)&addr,sizeof(addr));
  if(iRet < 0)
  {
	  return iRet;
  }
  return iRet;
}

int CTcpSocket::Bind(const char *lpIp, int iPort)
{
  struct sockaddr_in  addr;
  int iRet;
  
  m_iSock = socket( AF_INET, SOCK_STREAM, 0 ); 
  if(m_iSock < 0)
  {
    return -1;
  }
  
  addr.sin_family = AF_INET;
  //addr.sin_addr.s_addr = inet_addr(lpIp);
  addr.sin_addr.s_addr = htonl(INADDR_ANY);
  addr.sin_port = htons( iPort ); 
  
  iRet = bind( m_iSock,(struct sockaddr *)&addr,sizeof(addr));
  if(iRet < 0)
  {
	return iRet;
  }
  return iRet;
}

int CTcpSocket::Listen(int iNum)
{
  return listen(m_iSock, iNum);
}

int CTcpSocket::Accept(CTcpSocket &objSock , int iTimeOut)
{
  int iSock ,iReadNum = 0;
  struct   timeval tv;
  struct   timeval * ptv;
 
  if ( iTimeOut != TIME_INFINITE ) 
  {
    tv.tv_sec = iTimeOut;
    tv.tv_usec = 0;
    ptv = &tv;
  }
  else 
  {
    ptv = 0;
  }
  
  fd_set fdAccept;
  FD_ZERO(&fdAccept);
  FD_SET(m_iSock, &fdAccept);
  
  struct sockaddr_in  addr;
  socklen_t iLen;

  if(iReadNum = select(m_iSock + 1, &fdAccept, NULL, NULL, ptv))
  {
    iSock = accept(m_iSock, (struct sockaddr *)&addr, &iLen);
    if(0 > iSock) 
    {
      return -1;
    } 
  }
  else           /*超时*/
  {
    return -2;       
  }
  objSock.Attach(iSock); 
  return iSock;
  
}

int CTcpSocket::Close()
{
  return close(m_iSock);
}

int CTcpSocket::Send(const char *lpData, int iDataLen, int iTimeOut)
{
  int iRet, iCounter, iWantLen;

  iCounter = 0;
  iWantLen = iDataLen ;

  while ( iCounter != iDataLen ) 
  {
    if(IsReadyToWrite(iTimeOut)) 
    {
      iRet = write(m_iSock, lpData + iCounter, iWantLen);
      //printf("lpData=%s\n", lpData);
      if ( iRet == -1 ) 
      {
        if(errno == EPIPE)
          return 0;
        else
          return -1;
      }
      iCounter += iRet;
      iWantLen = iDataLen - iCounter;
    }
    else
      return -1;
  }
  return (iCounter);  
}

int CTcpSocket::Recv(char *lpData, int iDataLen, int iTimeOut)
{
  int iRet, iCounter, iWantLen;

  iCounter = 0;
  iWantLen = iDataLen ;

  while ( iCounter != iDataLen ) 
  {
    if(IsReadyToRead(iTimeOut)) 
    {
      iRet = read(m_iSock, lpData + iCounter,iWantLen);
      //printf("lpData=%s\n", lpData);
      if ( iRet <= 0 ) 
      {
         return iRet;
      }
      iCounter += iRet;
      iWantLen = iDataLen - iCounter;
    }
    else
    {
      return -1;
    }
  }
  return (iCounter);  
}

int CTcpSocket::Attach(int iSock)
{
  m_iSock = iSock;
  return 0;
}

int CTcpSocket::IsReadyToRead(int iTimeOut)
{
  struct   timeval tv;
  struct   timeval * ptv;
  fd_set   fdRead;

  int   iRet, len;

  if ( iTimeOut != TIME_INFINITE ) 
  {
    tv.tv_sec = iTimeOut;
    tv.tv_usec = 0;
    ptv = &tv;
  }
  else 
  {
    ptv = 0;
  }
  
  FD_ZERO(&fdRead);
  FD_SET(m_iSock, &fdRead);
  iRet = select(m_iSock + 1, &fdRead, 0, 0, ptv);
  switch ( iRet ) 
  {
    case 0: 
      return _FALSE_;  /*time expired */
    case -1:
      return _FALSE_; /* error occured */
  }

  if( FD_ISSET(m_iSock,&fdRead) )
    return _TRUE_;
  return _FALSE_;
}

int CTcpSocket::IsReadyToWrite(int iTimeOut)
{
  struct   timeval tv;
  struct   timeval * ptv;

  fd_set   fdRead;

  int   iRet, len;


  if ( iTimeOut != TIME_INFINITE ) 
  {
    tv.tv_sec = iTimeOut;
    tv.tv_usec = 0;
    ptv = &tv;
  }
  else 
  {
    ptv = 0;
  }

  FD_ZERO(&fdRead);
  FD_SET(m_iSock, &fdRead);
  iRet = select(m_iSock + 1, 0,&fdRead,  0, ptv);

  switch ( iRet ) {
    case 0: 
      return _FALSE_;  /*time expired */
    case -1:
      return _FALSE_; /* error occured */
  }
  if ( FD_ISSET(m_iSock,&fdRead) )
    return _TRUE_;
  return _FALSE_;
}

int CTcpSocket::GetHandle()
{
  return m_iSock;
}