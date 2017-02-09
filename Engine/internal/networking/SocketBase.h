#pragma once

#include <string>
#include <queue>

class SocketBase
{
public:
    SocketBase(std::string& inHostname, int inPort);

    bool isOpen();

    void setReceiveBufferLength(int newLength);

    virtual bool sendMsg(std::string message) = 0;

    virtual bool recvMsg() = 0;

protected:

    std::queue<std::string> queue;

	std::string hostname;

    int port;

    bool open;

	int receiveBufferLength;
};