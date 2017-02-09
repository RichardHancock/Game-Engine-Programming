#include "UNIXSocket.h"

#ifndef _WIN32

#include <netinet/in.h>
#include <netdb.h>
#include <cstring>
#include <unistd.h>
#include <fcntl.h>

#endif

#include "../../misc/Log.h"
#include "../../misc/Utility.h"

UNIXSocket::UNIXSocket(std::string &inHostname, int inPort)
        : SocketBase(inHostname, inPort)
{
#ifndef _WIN32
	FD_ZERO(&readSet);
	FD_ZERO(&writeSet);

	socketHandle = INVALID_SOCKET;

	struct addrinfo *result = nullptr, *ptr = nullptr, hints;
	int iResult = 0;

	//zero memory
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	std::string portString = Utility::intToString(port);
	iResult = getaddrinfo(hostname.c_str(), portString.c_str(), &hints, &result);
	if (iResult != 0)
	{
		Log::logE("Socket - getaddrinfo failed with error: " + getErrorMessage(iResult));
		return;
	}


	// Attempt to connect to an address until one succeeds
	for (ptr = result; ptr != NULL; ptr = ptr->ai_next)
	{
		// Create a SOCKET for connecting to server
		socketHandle = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
		if (socketHandle == INVALID_SOCKET)
		{
			iResult = errno; //Save error number before some other call overwrites it.
			Log::logE("Socket - socket() failed with error: " + getErrorMessage(iResult));
			freeaddrinfo(result);
			return;
		}

		fcntl(socketHandle, O_NONBLOCK);
		//Connect to server.
		iResult = connect(socketHandle, ptr->ai_addr, (int)ptr->ai_addrlen);
		if (iResult == SOCKET_ERROR)
		{
			close(socketHandle);
			socketHandle = INVALID_SOCKET;
			continue;
		}
		break;
	}

	freeaddrinfo(result);

	if (socketHandle == INVALID_SOCKET)
	{
		Log::logE("Socket - Unable to connect to server");
		return;
	}

	//Set timeout to 0 so all select functions return immediately and don't block.
	timeout.tv_sec = 0;
	timeout.tv_usec = 0;

	open = true;
#endif
}

bool UNIXSocket::sendMsg(std::string message)
{
#ifndef _WIN32
	int iResult;

	FD_SET(socketHandle, &writeSet);

	iResult = select(socketHandle + 1, NULL, &writeSet, NULL, &timeout);
	if (iResult > 0)
	{
		if (send(socketHandle, message.c_str(), message.length() + 1, 0) == SOCKET_ERROR)
		{
			iResult = errno; //Save error number before some other call overwrites it.
			Log::logE("Socket - send failed with error: " + getErrorMessage(iResult));
			return false;
		}
	}
	else if (iResult == SOCKET_ERROR)
	{
		iResult = errno; //Save error number before some other call overwrites it.
		Log::logE("Socket - select failed with error: " + getErrorMessage(iResult));
		return false;
	}

	FD_ZERO(&writeSet);

#endif

	return false;
}

bool UNIXSocket::recvMsg()
{
#ifndef _WIN32
	int iResult;
	char buffer[1024];

	FD_SET(socketHandle, &readSet);

	iResult = select(socketHandle+1, &readSet, NULL, NULL, &timeout);

	if (iResult > 0)
	{
		ssize_t recvSize = recv(socketHandle, buffer, 1024, 0);
		if (recvSize > 0)
		{
			Log::logI("Bytes received: " + Utility::intToString(iResult));
			Log::logI("Message: " + std::string(buffer));
		}
		else if (recvSize == 0)
		{
			Log::logI("Socket - Connection closed\n");
		}
		else
		{
			iResult = errno; //Save error number before some other call overwrites it.
			Log::logE("Socket - recv() failed with error: " + getErrorMessage(iResult));
		}
	}
	else if (iResult == SOCKET_ERROR)
	{
		iResult = errno; //Save error number before some other call overwrites it.
		Log::logE("Socket - select failed with error: " + getErrorMessage(iResult));
	}
#endif

	return false;
}

std::string UNIXSocket::getErrorMessage(int errorCode)
{
#ifndef _WIN32
	//This string may not be modified or deleted as the OS handles it.
	char* errorRaw = std::strerror(errorCode);

	std::string result(errorRaw);
	
	if (!result.empty())
		return result;
#endif 
	
	return std::string("Could not get error message for error code " + Utility::intToString(errorCode));
}
