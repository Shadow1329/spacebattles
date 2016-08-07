#pragma once
#include <set>
#include <iostream>
#include <fstream>

#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>
#include <websocketpp/common/thread.hpp>

#include <boost/lexical_cast.hpp>
#include <boost/date_time.hpp>


using websocketpp::connection_hdl;
using websocketpp::lib::placeholders::_1;
using websocketpp::lib::placeholders::_2;
using websocketpp::lib::bind;

typedef websocketpp::server<websocketpp::config::asio> WSServerT;


//Struct of connection data
struct ConnectionDataT
{
	int m_SessionId;		//ID of session
	std::string m_Name;		//Name of the Client
};

class ConnectionManager
{
public:
	ConnectionManager();
	~ConnectionManager();
	//Run server as application
	void Run();

	//Run server as daemon on port
	

protected:
	//Stop server
	void Control_Stop();

	//Kick user by ID
	void Control_Kick(int _id);

	//Main server menu
	void Control_ServerMenu();

	//Log writer
	void Util_WriteLog(std::string _msg);


	//Handlers
	void Handle_Open(connection_hdl _hdl);

	void Handle_Close(connection_hdl _hdl);

	void Handle_Message(connection_hdl _hdl, WSServerT::message_ptr _msg);


private:
	typedef std::map<connection_hdl, ConnectionDataT, std::owner_less<connection_hdl>> ConListT;


	//WebSocket Server
	WSServerT m_Server;

	//Server port
	uint16_t m_Port;

	//Server state
	bool m_ServerState;

	//Thread
	websocketpp::lib::shared_ptr<websocketpp::lib::thread> m_Thread;

	//Connections
	ConListT m_Connections;
	int m_NextSessionId;
	int m_MaxConnections;
};

