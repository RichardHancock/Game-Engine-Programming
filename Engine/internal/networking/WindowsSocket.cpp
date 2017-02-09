#include "WindowsSocket.h"

#include "../../misc/Log.h"
#include "../../misc/Utility.h"

WindowsSocket::WindowsSocket(std::string &inHostname, int inPort)
        : SocketBase(inHostname, inPort)
{
#ifdef _WIN32
    connectSocket = INVALID_SOCKET;

	FD_ZERO(&readSet);
	FD_ZERO(&writeSet);

	struct addrinfo *result = NULL, *ptr = NULL, hints;

	//queue.

	int iResult;

	iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
	if (iResult != 0)
	{
		Log::logE("Socket - WSAStartup failed with error: " + Utility::intToString(iResult));
		return;
	}

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	std::string portString = Utility::intToString(port);

	// Resolve the server address and port
	iResult = getaddrinfo(hostname.c_str(), portString.c_str(), &hints, &result);
	if (iResult != 0)
	{
		Log::logE("Socket - getaddrinfo failed with error: " + Utility::intToString(iResult));
		WSACleanup();
		return;
	}

	// Attempt to connect to an address until one succeeds
	for (ptr = result; ptr != NULL; ptr = ptr->ai_next)
	{
		// Create a SOCKET for connecting to server
		connectSocket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
		if (connectSocket == INVALID_SOCKET)
		{
			Log::logE("Socket - socket() failed with error: " + Utility::intToString(WSAGetLastError()));
			freeaddrinfo(result);
			WSACleanup();
			return;
		}

		//Connect to server.
		iResult = connect(connectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
		if (iResult == SOCKET_ERROR)
		{
			closesocket(connectSocket);
			connectSocket = INVALID_SOCKET;
			continue;
		}
		break;
	}

	freeaddrinfo(result);

	if (connectSocket == INVALID_SOCKET)
	{
		Log::logE("Socket - Unable to connect to server");
		WSACleanup();
		return;
	}

	unsigned long nonBlocking = 1;
	iResult = ioctlsocket(connectSocket, FIONBIO, &nonBlocking);
	if (iResult == SOCKET_ERROR)
	{
		Log::logE("Socket - Setting socket to non-blocking failed: " + Utility::intToString(WSAGetLastError()));
	}

	//Set timeout to 0 so all select functions return immediately and don't block.
	timeout.tv_sec = 0;
	timeout.tv_usec = 0;

	open = true;

#endif
}

bool WindowsSocket::sendMsg(std::string message)
{
#ifdef _WIN32
	int iResult;

	FD_SET(connectSocket, &writeSet);

	iResult = select(0, NULL, &writeSet, NULL, &timeout);
	if (iResult > 0)
	{
		iResult = send(connectSocket, message.c_str(), message.length() + 1, 0);
		if (iResult == SOCKET_ERROR)
		{
			Log::logE("Socket - send failed with error: " + Utility::intToString(WSAGetLastError()));
			return false;
		}
	}
	else if (iResult == SOCKET_ERROR)
	{
		Log::logE("Socket - select failed with error: " + Utility::intToString(WSAGetLastError()));
		return false;
	}

	FD_ZERO(&writeSet);

#endif

	return false;
}

bool WindowsSocket::recvMsg()
{
#ifdef _WIN32
	int iResult;
	char buffer[1024];

	FD_SET(connectSocket, &readSet);

	iResult = select(0, &readSet, NULL, NULL, &timeout);

	if (iResult > 0)
	{
		iResult = recv(connectSocket, buffer, 1024, 0);
		if (iResult > 0)
		{
			Log::logI("Bytes received: " + Utility::intToString(iResult));
			Log::logI("Message: " + std::string(buffer));
		}
		else if (iResult == 0)
		{
			Log::logI("Socket - Connection closed\n");
		}
		else
		{
			Log::logE("Socket - recv() failed with error: " + Utility::intToString(WSAGetLastError()));
		}
	}
	else if (iResult == SOCKET_ERROR)
	{
		Log::logE("Socket - select failed with error: " + Utility::intToString(WSAGetLastError()));
	}
#endif

	return false;
}
