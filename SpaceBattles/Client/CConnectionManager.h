#pragma once

#include <iostream>
#include <fstream>

#include <websocketpp/config/asio_no_tls_client.hpp>
#include <websocketpp/client.hpp>
#include <websocketpp/common/thread.hpp>

#include <boost/thread/thread.hpp>
#include <boost/date_time.hpp>




//Connection manager class
class CConnectionManager
{
public:
	CConnectionManager();
	~CConnectionManager();
};

