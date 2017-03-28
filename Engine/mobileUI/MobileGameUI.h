#pragma once

#ifdef _WIN32
#pragma warning(push, 0)
#include <sio_socket.h>
#include <sio_client.h>
#pragma warning(pop)
#else
#include <sio_socket.h>
#include <sio_client.h>
#endif

#include <queue>

/** @brief	A class that allows the sending and receiving events from WebSocket clients. */
class MobileGameUI
{
public:

	/** @brief	Default constructor. */
	MobileGameUI();

	/** @brief	Destructor. */
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

	/**
	 @brief	Gets session identifier that should be used by clients to connect to the game.
	
	 @return	The session identifier or an empty string if not connected yet.
	 */
	std::string getSessionID() const { return sessionID; };

	/** @brief	Closes any open connection. */
	void close();

	/**
	 @brief	Pull the oldest event off the received event queue.
	
	 @return	A std::string describing the event, or an empty string if the queue is empty.
	 */
	std::string pollQueue();

	/**
	 @brief	Queries if the queue is empty.
	
	 @return	True if the queue is empty, false if not.
	 */
	bool isQueueEmpty() const { return eventQueue.empty(); };

	/**
	 @brief	Send a message to all connected clients with type 'gameOutput'.
	
	 @param	message	The message.
	 */
	void send(std::string message);

	/**
	 @brief	Send a message to all connected clients with type provided by parameter 'eventName'.
	
	 @param	eventName	Name of the event type .
	 @param	message  	The message.
	 */
	void send(std::string eventName, std::string message);

private:

	/** @brief	Callback function triggered when the connection is established. */
	void onConnected();

	/**
		@brief	Callback function triggered when the connection is lost. *
	
		@param	reason	The reason of the disconnect.
	 */
	void onClose(sio::client::close_reason const & reason);

	/**
		@brief	Callback function triggered when a fail condition is met during the connection
		The 3rd party library rarely triggers this and provides no info on the error.
		So pretty much useless, just handle like a disconnect
	 */
	void onFail();

	/** @brief	Handles events related to connections, disconnections and errors. */
	sio::client client;

	/** @brief	Handles user generated events. */
	sio::socket::ptr socket;

	/** @brief	Contains the current session ID or an empty string if no session has been created. */
	std::string sessionID;

	/** @brief	Queue of all the received data from the server. */
	std::queue<std::string> eventQueue;

	/** @brief	Clears the event queue. */
	void clearEventQueue();

	/** @brief	Binds event callback functions for the types of events this library handles. */
	void bindEvents();
};