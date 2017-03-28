#include "MobileGameUI.h"

#include <iostream>

#include "../misc/Log.h"

MobileGameUI::MobileGameUI()
	: socket(nullptr)
{
	client.set_open_listener(std::bind(&MobileGameUI::onConnected, this));
	client.set_close_listener(std::bind(&MobileGameUI::onClose, this, std::placeholders::_1));
	client.set_fail_listener(std::bind(&MobileGameUI::onFail, this));
}

MobileGameUI::~MobileGameUI()
{
	if (socket != nullptr)
		socket->close();
}

void MobileGameUI::connect(std::string address)
{
	client.connect(address);
}

void MobileGameUI::close()
{
	socket->close();
	//Should trigger onClose event
}

std::string MobileGameUI::pollQueue()
{
	if (eventQueue.empty())
		return "";

	std::string event = eventQueue.front();
	eventQueue.pop();
	return event;
}

void MobileGameUI::send(std::string message)
{
	if (isConnected() && socket != nullptr)
	{
		sio::message::ptr data = sio::object_message::create();
		data->get_map()["room"] = sio::string_message::create(sessionID);
		data->get_map()["msg"] = sio::string_message::create(message);

		socket->emit("gameOutput", data);
	}
}

void MobileGameUI::send(std::string eventName, std::string message)
{
	if (isConnected() && socket != nullptr)
	{
		sio::message::ptr data = sio::object_message::create();
		data->get_map()["room"] = sio::string_message::create(sessionID);
		data->get_map()["msg"] = sio::string_message::create(message);

		socket->emit(eventName, data);
	}
}

void MobileGameUI::onConnected()
{
	socket = client.socket();
	Log::logI("Connection Established");

	bindEvents(); //This sets all the callback functions to handle messages from the server

	Log::logI("Requesting a Session ID from the server");
	// The data has no significance, maybe replaced with a Game ID in the future
	socket->emit("newGameSessionRequest", std::string("1")); 
}

void MobileGameUI::onClose(sio::client::close_reason const & reason)
{
	//Reset Socket to nullptr so no functions attempt to access it.
	socket = nullptr;
	//Reset Session ID as it is now invalid
	sessionID = "";
	//Clear queue
	clearEventQueue();


	std::string reasonDesc;

	//Used a switch as more maybe added in future
	switch (reason)
	{
	case sio::client::close_reason_normal:
		reasonDesc = "safely";
		break;
	case sio::client::close_reason_drop:
		reasonDesc = "due to a drop of connection";
		break;
	}

	Log::logI("MobileGameUI connection has been closed " + reasonDesc);
}

void MobileGameUI::onFail()
{
	//Can't seem to find a function to get more information on the fail condition
	Log::logE("MobileGameUI has encountered a fail condition inside sio::client");
}

void MobileGameUI::clearEventQueue()
{
	//Reasonably efficient way to clear queue: http://stackoverflow.com/a/709161/3262098
	std::queue<std::string> empty;
	std::swap(eventQueue, empty);
}

void MobileGameUI::bindEvents()
{
	socket->on("newGameSessionResponse", sio::socket::event_listener_aux([&](std::string const&, sio::message::ptr const& data, bool, sio::message::list&)
	{
		if (data->get_map()["room"]->get_flag() == data->flag_string)
		{
			std::cout << "Message Received: " << data->get_map()["room"]->get_string() << std::endl;;

			sessionID = data->get_map()["room"]->get_string();
		}
		else
		{
			std::cout << "Error, expected string got enum: " << data->get_flag() << std::endl;
		}
	}));

	socket->on("gameInput", sio::socket::event_listener_aux([&](std::string const&, sio::message::ptr const& data, bool, sio::message::list&)
	{
		if (data->get_map()["msg"]->get_flag() == data->flag_string)
		{
			std::cout << "Message Received: " << data->get_map()["msg"]->get_string() << std::endl;
			eventQueue.push( data->get_map()["msg"]->get_string());
		}
		else
		{
			std::cout << "Error, expected string got enum: " << data->get_flag() << std::endl;
		}
	}));
}
