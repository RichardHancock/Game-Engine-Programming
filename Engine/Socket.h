#pragma once

#include <string>

#include "internal/networking/SocketBase.h"

class Socket
{
public:
	Socket(std::string inHostname, int inPort);

	bool isOpen();

	bool sendMsg(std::string message);

	bool recvMsg();
private:

	SocketBase* socket;
};
