#pragma once

#include "SocketBase.h"

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#endif

/** @brief	The WinSock Wrapper. */
class WindowsSocket : public SocketBase
{
public:

    /**
     @brief	Constructor.
    
     @param [in,out]	inHostname	The hostname.
     @param 			inPort	  	The port.
     */
    WindowsSocket(std::string &inHostname, int inPort);

    /**
     @brief	Sends a message.
    
     @param	message	The message.
    
     @return	True if it succeeds, false if it fails.
     */
    virtual bool sendMsg(std::string message);

    /**
     @brief	Determines if we can receive message.
    
     @return	True if it succeeds, false if it fails.
     */
    virtual bool recvMsg();

protected:

	/**
	 @brief	Gets WinSock error message.
	
	 @param	errorCode	The error code.
	
	 @return	The error message.
	 */
	std::string getErrorMessage(int errorCode);

#ifdef _WIN32
    /** @brief	Internal WinSock Data. */
    WSADATA wsaData;

	/** @brief	The connection socket. */
	SOCKET connectSocket;

	/** @brief	Internal WinSock Data. */
	FD_SET readSet;
	/** @brief	Internal WinSock Data. */
	FD_SET writeSet;

	/** @brief	The timeout (0 timeout because we want non-blocking). */
	TIMEVAL timeout;
#endif
};


