#pragma once

#include <iostream>
#include <fstream>

#include <websocketpp/config/asio_no_tls_client.hpp>
#include <websocketpp/client.hpp>
#include <websocketpp/common/thread.hpp>

#include <boost/thread/thread.hpp>
#include <boost/date_time.hpp>




using websocketpp::connection_hdl;
using websocketpp::lib::placeholders::_1;
using websocketpp::lib::placeholders::_2;
using websocketpp::lib::bind;




typedef websocketpp::client<websocketpp::config::asio_client> WSClientT;




//Connection manager class
class CConnectionManager
{
private:
	//WebSocket Client
	WSClientT m_Client;

	//Connection handle
	websocketpp::connection_hdl m_Hdl;

	//Indicates state of connection
	bool m_Connection_Succsesful;

	//Thread
	websocketpp::lib::shared_ptr<websocketpp::lib::thread> m_Thread;

	//Address of server
	std::string m_Address;



public:
	CConnectionManager();
	~CConnectionManager();

	//Start client
	void Start();


protected:
	//Connect to server with _uri
	void Control_Connect(std::string const & _uri);

	//Diconnect from server
	void Control_Disconnect();

	//Send message
	void Control_Send(std::string const & _msg);

	//Chat menu
	void Control_Chat();

	//Log writer
	void Util_WriteLog(std::string _msg);


	//Callbacks
	void onOpen(connection_hdl _hdl);

	void onClose(connection_hdl _hdl);

	void onFail(connection_hdl _hdl);

	void onMessage(connection_hdl _hdl, WSClientT::message_ptr _msg);
};