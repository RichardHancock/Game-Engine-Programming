#include "Socket.h"

#include "misc/Log.h"
#include "misc/Utility.h"
#include "internal/networking/WindowsSocket.h"
#include "internal/networking/UNIXSocket.h"

Socket::Socket(std::string inHostname, int inPort)
{
	socket = nullptr;

#ifdef _WIN32
	Log::logI("Socket - Using WinSock V2.2");
	socket = new WindowsSocket(inHostname, inPort);
#else
	Log::logI("Socket - Using Berkeley Sockets");
	socket = new UNIXSocket(inHostname, inPort);
#endif
}

bool Socket::isOpen()
{
	return (socket != nullptr ? socket->isOpen() : false);
}

bool Socket::sendMsg(std::string message)
{
	return (isOpen() ? socket->sendMsg(message) : false);
}

bool Socket::recvMsg()
{
	return (isOpen() ? socket->recvMsg() : false);
}
