#pragma once

#include <iostream>
#include <vector>
#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>
#include <websocketpp/common/thread.hpp>
#include "UserInformation.h"

using websocketpp::connection_hdl;
using namespace std;

struct ConnectionDataT
{
	int m_SessionId;		//ID of session
	std::string m_Name;		//Name of the Client
};



class Constant
{



private:

	//typedef std::map<connection_hdl, ConnectionDataT, std::owner_less<connection_hdl>> ConListT;
	typedef std::map<connection_hdl, ConnectionDataT, std::owner_less<connection_hdl>> ConListT;
	ConListT m_Connections;


	int id;
	static Constant* _constant;

	vector<connection_hdl> vectorConnection_hdl;

public:
	Constant();
	

	static Constant* getInstance()
	{
		if (!_constant)
		{
			_constant = new Constant;
		}
		
		return _constant;
	}

	 void addConnectionHDL(ConnectionDataT data,connection_hdl _hdl)
	{
		//vectorConnection_hdl.push_back(_hdl);
		m_Connections[_hdl] = data;
	}


	 void ReplaceUser(connection_hdl _hdl, UserInformation* userInformation)
	 {
		 m_Connections[_hdl].m_Name = userInformation->getUserName();
		 m_Connections[_hdl].m_SessionId = userInformation->getUserID();

	 }


	 void setNameUser(connection_hdl _hdl, std::string name)
	 {
		 m_Connections[_hdl].m_Name = name;

	 }


	 ConListT getM_Connetcions()
	 {
		 return m_Connections;
	 }

	int getIdForPlayer()
	{
		return id++;
	}

	int  findAndDelete(connection_hdl _hdl)
	{
		auto it = m_Connections.find(_hdl);

		

		if (it != m_Connections.end())
		{
			std::cout << "User \"" << Constant::getInstance()->getM_Connetcions()[_hdl].m_Name << "\" with ID " << Constant::getInstance()->getM_Connetcions()[_hdl].m_SessionId << " disconnected." << std::endl;

			//Write log
			//Util_WriteLog("User \"" + Constant::getInstance()->getM_Connetcions()[_hdl].m_Name + "\" with ID " + std::to_string(Constant::getInstance()->getM_Connetcions()[_hdl].m_SessionId) + " disconnected.");

			//Delecte connection from map
			m_Connections.erase(_hdl);
		}
		return m_Connections[_hdl].m_SessionId;
	}

	int getID(connection_hdl _hdl)
	{
		return m_Connections[_hdl].m_SessionId;

	}
};

