#pragma once

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <string>
#include <queue>

class Socket
{
public:
	Socket(int inPort, std::string inHostname);

	bool isOpen();

	bool sendMsg(std::string message);

	bool recvMsg();
private:

	WSADATA wsaData;
	SOCKET connectSocket;

	FD_SET readSet;
	FD_SET writeSet;

	TIMEVAL timeout;

	std::queue<std::string> queue;
	
	const int recvbuflen = 256;

	bool open;

	std::string hostname;
	int port;
};