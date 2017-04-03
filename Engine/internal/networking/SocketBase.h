#pragma once

#include <string>
#include <queue>

/** @brief	A socket base class. */
class SocketBase
{
public:

    /**
     @brief	Constructor.
    
     @param [in,out]	inHostname	The hostname.
     @param 			inPort	  	The port.
     */
    SocketBase(std::string& inHostname, int inPort);

    /**
     @brief	Query if this Socket is open.
    
     @return	True if open, false if not.
     */
    bool isOpen();

    /**
     @brief	Sets receive buffer length.
    
     @param	newLength	Length of the buffer.
     */
    void setReceiveBufferLength(int newLength);

	/**
	@brief	Sends a message.

	@param	message	The message.

	@return	True if it succeeds, false if it fails.
	*/
    virtual bool sendMsg(std::string message) = 0;

	/**
	@brief	Determines if we can receive message.

	@return	True if it succeeds, false if it fails.
	*/
    virtual bool recvMsg() = 0;

protected:

    /** @brief	The queue of received messages. */
    std::queue<std::string> queue;

	/** @brief	The hostname. */
	std::string hostname;

    /** @brief	The port. */
    int port;

    /** @brief	True if open. */
    bool open;

	/** @brief	Length of the receive buffer. */
	int receiveBufferLength;
};