#include "SocketBase.h"

SocketBase::SocketBase(std::string& inHostname, int inPort)
    :  hostname(inHostname), port(inPort), open(false), receiveBufferLength(256)
{

}

bool SocketBase::isOpen()
{
    return open;
}

void SocketBase::setReceiveBufferLength(int newLength)
{
    receiveBufferLength = newLength;
}
