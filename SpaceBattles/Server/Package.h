#pragma once
#include <stdio.h>
#include <string>
#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>
#include <websocketpp/common/thread.hpp>
#include "Constant.h"
using websocketpp::connection_hdl;
class Package
{
private:
	std::string command;
	std::string body;
	std::string message;
	connection_hdl _hdl;

	
public:
	Package()
	{
		command = "";
		body = "";
		message = "";
	}

	Package(std::string command, std::string body, std::string message, connection_hdl _hdl)
	{
		this->body = body;
		this->message = message;
		this->command = command;
		this->_hdl = _hdl;

	}
	std::string getCommand()
	{
		return command;
	};

	std::string getBody()
	{
		return body;
	};

	std::string getMessage()
	{
		return message;
	};

	connection_hdl getHDL()
	{
		return _hdl;
	}
	

};