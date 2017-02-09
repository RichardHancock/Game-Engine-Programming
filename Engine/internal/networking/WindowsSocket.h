#pragma once

#include "SocketBase.h"

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#endif

class WindowsSocket : public SocketBase
{
public:
    WindowsSocket(std::string &inHostname, int inPort);

    virtual bool sendMsg(std::string message);

    virtual bool recvMsg();

protected:
	std::string getErrorMessage(int errorCode);

#ifdef _WIN32
    WSADATA wsaData;
	SOCKET connectSocket;

	FD_SET readSet;
	FD_SET writeSet;

	TIMEVAL timeout;
#endif
};


