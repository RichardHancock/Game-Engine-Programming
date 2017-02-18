#pragma once

#include <sio_socket.h>
#include <sio_client.h>
#include <queue>

class MobileGameUI
{
public:

	MobileGameUI();

	~MobileGameUI();

	/**
	@brief	Connects to the given URL or IP address.
	onConnected Function will be triggered if a connection is established
	onFail will trigger if a connection cannot be made

	@param	address	The address to connect.
	 */
	void connect(std::string address);

	/**
	 @brief	Query if class is connected to server.
	
	 @return	True if connected, false if not.
	 */
	bool isConnected() const { return client.opened(); };

	std::string getSessionID() const { return sessionID; };

	void close();

	std::string pollQueue();

	bool isQueueEmpty() const { return eventQueue.empty(); };

	void send(std::string message);

private:

	void onConnected();

	void onClose(sio::client::close_reason const & reason);

	void onFail();

	//Handles events related to connections, disconnections and errors
	sio::client client;

	//Handles user generated events.
	sio::socket::ptr socket;

	//Contains the current session ID or an empty string if no session has been created
	std::string sessionID;

	/** @brief	Queue of all the received data from the server. */
	std::queue<std::string> eventQueue;

	void clearEventQueue();

	void bindEvents();
};