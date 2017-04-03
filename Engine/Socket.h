#pragma once

#include <string>

#include "internal/networking/SocketBase.h"

/** @brief	A front-end socket that hides the complex multi-platform back-end. */
class Socket
{
public:

	/**
	 @brief	Constructor.
	
	 @param	inHostname	The hostname.
	 @param	inPort	  	The port.
	 */
	Socket(std::string inHostname, int inPort);
#
	/**
	@brief	Query if this Socket is open.

	@return	True if open, false if not.
	*/
	bool isOpen();

	/**
	@brief	Sends a message.

	@param	message	The message.

	@return	True if it succeeds, false if it fails.
	*/
	bool sendMsg(std::string message);

	/**
	@brief	Determines if we can receive message.

	@return	True if it succeeds, false if it fails.
	*/
	bool recvMsg();
private:

	/** @brief	The internal socket. */
	SocketBase* socket;
};
