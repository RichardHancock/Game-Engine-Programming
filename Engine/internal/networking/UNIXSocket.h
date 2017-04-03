#pragma once

#include "SocketBase.h"

#ifndef _WIN32

#include <sys/socket.h>
#include <sys/types.h>
#include <sys/time.h>

#endif

/** @brief	The BSD Sockets Wrapper */
class UNIXSocket : public SocketBase
{
public:

    /**
     @brief	Constructor.
    
     @param [in,out]	inHostname	The hostname.
     @param 			inPort	  	The port.
     */
    UNIXSocket(std::string &inHostname, int inPort);

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
	@brief	Gets BSD Sockets error message.

	@param	errorCode	The error code.

	@return	The error message.
	*/
	std::string getErrorMessage(int errorCode);

#ifndef _WIN32
	/** @brief	The timeout (0 timeout because we want non-blocking). */
	timeval timeout;

	/** @brief	The connection socket. */
	int socketHandle;

	/** @brief	Internal BSD Socket Data. */
	fd_set readSet;
	/** @brief	Internal BSD Socket Data. */
	fd_set writeSet;

	/** @brief	An invalid socket constant. */
	const int INVALID_SOCKET = -1;

	/** @brief	A socket error constant. */
	const int SOCKET_ERROR = -1;
#endif
};
