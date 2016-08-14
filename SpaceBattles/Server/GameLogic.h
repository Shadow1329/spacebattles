#pragma once
#include <iostream>
#include <vector>
#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>
#include <websocketpp/common/thread.hpp>
#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>
#include <websocketpp/common/thread.hpp>

#include <boost/lexical_cast.hpp>
#include <boost/date_time.hpp>


using websocketpp::connection_hdl;
using namespace std;
using websocketpp::lib::placeholders::_1;
using websocketpp::lib::placeholders::_2;
using websocketpp::lib::bind;




class GameLogic
{
private:
	struct ConnectionDataT {
		std::string message;
		int id;
	};
	typedef websocketpp::server<websocketpp::config::asio> WSServerT;
	typedef std::map<connection_hdl, ConnectionDataT, std::owner_less<connection_hdl>> ConListT;
	ConListT m_Connections;
	static GameLogic* gameLogic;
	WSServerT* m_Server;
public:
	GameLogic();
	~GameLogic();

	static GameLogic* getInstance();


	void setServer(WSServerT* m_ServerS)
	{
		m_Server = m_ServerS;
	}

	void updateClient(connection_hdl _hdl,  std::string message)
	{
		
		m_Connections[_hdl].message = message;
	}

	void deleteClient(connection_hdl _hdl)
	{
		m_Connections.erase(_hdl);
	}
	void addClient(connection_hdl _hdl, int id)
	{
		m_Connections[_hdl].id = id;
		

		for (auto it1 : m_Connections)
		{
			if (it1.second.id == id)
			{
				for (auto it : m_Connections)
				{
					if (it.second.id != id)
					{
						m_Server->send(it1.first, "create_object;" + std::to_string(it.second.id), websocketpp::frame::opcode::text);// +it.second.message, websocketpp::frame::opcode::text);

					}
				}
			}
		} 
		for (auto it : m_Connections)
		{
			if (it.second.id == id)
			{
				m_Server->send(it.first, "create_object_ready;" + std::to_string(id), websocketpp::frame::opcode::text);
				return;
			}
		}
	}



};

