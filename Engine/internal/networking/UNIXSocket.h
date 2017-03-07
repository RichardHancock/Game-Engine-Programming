#pragma once

#include "SocketBase.h"

#ifndef _WIN32

#include <sys/socket.h>
#include <sys/types.h>
#include <sys/time.h>

#endif

class UNIXSocket : public SocketBase
{
public:
    UNIXSocket(std::string &inHostname, int inPort);

    virtual bool sendMsg(std::string message);

    virtual bool recvMsg();

protected:
	std::string getErrorMessage(int errorCode);

#ifndef _WIN32
	timeval timeout;

	int socketHandle;

	fd_set readSet;
	fd_set writeSet;

	const int INVALID_SOCKET = -1;
	const int SOCKET_ERROR = -1;
#endif
};
