#include "ConnectionManager.h"
#include "Constant.h"
#include "ParserCreateObject.h"
#include "ParserUpdateObject.h"
 

int l = 0;


ConnectionManager::ConnectionManager()
{
	parserController = new ParserController();
	parserController->setNextParserController(new ParserMessage());
	parserController->setNextParserController(new ParserAddNewUser());
	parserController->setNextParserController(new ParserCreateObject());
	parserController->setNextParserController(new ParserUpdateObject());


	//Default server port is 9002
	m_Port = 9002;

	//Initial server state
	m_ServerState = false;

	//First session ID
	m_NextSessionId = 1;

	//Setting maximum of connections
	m_MaxConnections = 10;

	//Clear connection container
	//Constant::getInstance()->getM_Connetcions().clear();

	// Configure log messages
	m_Server.clear_access_channels(websocketpp::log::alevel::all);
	m_Server.clear_error_channels(websocketpp::log::elevel::all);
	//m_Server.set_access_channels(websocketpp::log::alevel::connect);
	//m_Server.set_access_channels(websocketpp::log::alevel::disconnect);

	// Initialize ASIO
	m_Server.init_asio();

	//Start point of thread
	m_Server.start_perpetual();

	// Register our handlers
	m_Server.set_open_handler(bind(&ConnectionManager::Handle_Open, this, ::_1));
	m_Server.set_close_handler(bind(&ConnectionManager::Handle_Close, this, ::_1));
	m_Server.set_message_handler(bind(&ConnectionManager::Handle_Message, this, ::_1, ::_2));
	GameLogic::getInstance()->setServer(&m_Server);

	// Creating thread
	m_Thread = websocketpp::lib::make_shared<websocketpp::lib::thread>(&WSServerT::run, &m_Server);

}


ConnectionManager::~ConnectionManager()
{

	//End point of thread
	m_Server.stop_perpetual();

	m_Thread->join();
}

void ConnectionManager::Run()
{
	std::string input;

	//Ask user to write port
	std::cout << "Enter port 1-65535 or press Enter to keep dafault port(" << m_Port << "):" << std::endl;
	while (1)
	{
		int bufid;

		std::getline(std::cin, input);
		if (input != "")
		{
			//Is it correct?
			try
			{
				bufid = boost::lexical_cast<int>(input);
				if ((bufid > 0) && (bufid < 65536))
				{
					m_Port = bufid;
					break;
				}
				else std::cout << "Incorrect PORT. Enter number 1-65535." << std::endl;
			}
			catch (const boost::bad_lexical_cast &)
			{
				std::cout << "Incorrect PORT. Enter number 1-65535." << std::endl;
			}
		}
		else break;
	}

	//Start server
	try
	{
		//Start to listen port and to accept connections
		m_Server.listen(m_Port);
		m_Server.start_accept();

		//Set server state to true
		m_ServerState = true;

		//Write log
		Util_WriteLog("Server is running on port \"" + std::to_string(m_Port) + "\".");

		std::cout << "Server is running on port " << m_Port << ". Waiting for connections..." << std::endl;
		std::cout << "Write /help to show commands." << std::endl;

		//Start server main menu
		Control_ServerMenu();
	}
	catch (websocketpp::exception const & e)
	{
		//Write log
		Util_WriteLog("Error. Another server is running on port \"" + std::to_string(m_Port) + "\".");

		//Print error
		std::cout << "Another server is running on this port." << std::endl;
		std::cin.get();
	}
}


// Run server as daemon



//Stop server
void ConnectionManager::Control_Stop()
{
	std::cout << "Stop server." << std::endl;

	//Write log
	Util_WriteLog("Server was stopped.");

	//Inform everybody about end of connection

	
	for (auto it : Constant::getInstance()->getM_Connetcions())
	{
		m_Server.send(it.first, "[Server]: Server is shutting down. Good buy! :)", websocketpp::frame::opcode::text);
		m_Server.close(it.first, websocketpp::close::status::blank, "");
	}

	//Stop server
	m_Server.stop();
	m_Server.stop_listening();
	m_ServerState = false;
}


//Kick user by session id
void ConnectionManager::Control_Kick(int _id)
{
	//If user with this ID exists
	for (auto it : Constant::getInstance()->getM_Connetcions())
	{
		if (it.second.m_SessionId == _id)
		{
			std::cout << "User with ID = " << it.second.m_SessionId << "was kicked." << std::endl;

			//Write log
			Util_WriteLog("User with ID = " + std::to_string(it.second.m_SessionId) + "was kicked.");

			//Inform and kick user
			m_Server.send(it.first, "You were kicked from server.", websocketpp::frame::opcode::text);
			m_Server.close(it.first, websocketpp::close::status::blank, "");
			return;
		}
	}
	std::cout << "There is no user with such ID." << std::endl;
}


//Server menu
void ConnectionManager::Control_ServerMenu()
{
	std::string input;

	while (m_ServerState)
	{
		std::getline(std::cin, input);

		//If command - stop
		if (input == "/stop")
		{
			//Stop server
			Control_Stop();
		}

		//If command - help
		else if (input == "/help")
		{
			//Show all commands
			std::cout << "Commands:" << std::endl;
			std::cout << "/stop - stop server." << std::endl;
			std::cout << "/help - help with commands." << std::endl;
			std::cout << "/show - show connections." << std::endl;
			std::cout << "/kick ID - disconnect user by ID." << std::endl;
		}

		//If command - show
		else if (input == "/show")
		{
			//Show all connections
			std::cout << "Connections list:" << std::endl;
			for (auto it : Constant::getInstance()->getM_Connetcions())
			{
				std::cout << "User \"" << it.second.m_Name << "\" with ID = " << it.second.m_SessionId << "." << std::endl;
			}
		}

		//If command - kick
		else if (input.substr(0, input.find(' ')) == "/kick")
		{
			//Kick user by ID
			int bufid;
			try
			{
				bufid = boost::lexical_cast<int>(input.substr(input.find(' ') + 1, input.length()));
				Control_Kick(bufid);
			}
			catch (const boost::bad_lexical_cast &)
			{
				std::cout << "Incorrect ID." << std::endl;
			}
		}

		//Else - incorrect input
		else std::cout << "Incorrect command." << std::endl;
	}
}


//Write log
void ConnectionManager::Util_WriteLog(std::string _msg)
{
	std::string filename = "server_log.txt";

	//Open log file
	std::ofstream fout(filename, std::ios_base::app);

	//Write string
	fout << "[" << boost::posix_time::second_clock::local_time() << "]: " << _msg << std::endl;

	//Close file
	fout.close();
}


//Open connection(if client succesfully connect to server)
void ConnectionManager::Handle_Open(connection_hdl _hdl)
{
	ConnectionDataT data;

	//if there is maximum of connections, abort connection
	if (Constant::getInstance()->getM_Connetcions().size() == m_MaxConnections)
	{
		//Inform and close connection
		m_Server.send(_hdl, "Sorry, the server is full :(. Try again later.", websocketpp::frame::opcode::text);
		m_Server.close(_hdl, websocketpp::close::status::blank, "");

		//Write log
		Util_WriteLog("Someone try to connect to server, but the server is full. Abort connection.");
	}

	//else add connection to list
	else
	{
		// Fill data
		data.m_SessionId = Constant::getInstance()->getIdForPlayer();
		data.m_Name = "";
		//bool g = Constant::getInstance()->getM_Connetcions();
		Constant::getInstance()->addConnectionHDL(data,_hdl);
		for (auto it : Constant::getInstance()->getM_Connetcions())
		{
			
			if (Constant::getInstance()->getID(_hdl) != it.second.m_SessionId)
			{
				cout << "create_object;" + it.second.m_SessionId << endl;
				m_Server.send(it.first, "create_object;" + std::to_string(Constant::getInstance()->getID(_hdl)) , websocketpp::frame::opcode::text);
			}
			else
			{
				cout << "connection_accepted;" + it.second.m_SessionId << endl;
				//m_Server.send(it.first, "connection_accepted;" + std::to_string(Constant::getInstance()->getID(_hdl)) , websocketpp::frame::opcode::text);
			}

		}
		GameLogic::getInstance()->addClient(_hdl, data.m_SessionId);
		
		//Attach data to connection
		//auto g = Constant::getInstance()->getM_Connetcions();// [_hdl] = data;

		//Write log
		Util_WriteLog("Someone try to connect to server, wating for \"hello\" message.");
	}
}


//Close connection(if client disconnect from server)
void ConnectionManager::Handle_Close(connection_hdl _hdl)
{
	int id = Constant::getInstance()->findAndDelete(_hdl);
	//If connection exists
	/*auto it = Constant::getInstance()->getM_Connetcions().find(_hdl);
	if (it != Constant::getInstance()->getM_Connetcions().end())
	{
		std::cout << "User \"" << Constant::getInstance()->getM_Connetcions()[_hdl].m_Name << "\" with ID " << Constant::getInstance()->getM_Connetcions()[_hdl].m_SessionId << " disconnected." << std::endl;

		//Write log
		Util_WriteLog("User \"" + Constant::getInstance()->getM_Connetcions()[_hdl].m_Name + "\" with ID " + std::to_string(Constant::getInstance()->getM_Connetcions()[_hdl].m_SessionId) + " disconnected.");

		//Delecte connection from map
		Constant::getInstance()->getM_Connetcions().erase(_hdl);
	}*/

	for (auto it : Constant::getInstance()->getM_Connetcions())
	{
		if (it.second.m_SessionId != id)
		{
			m_Server.send(it.first, "delete_object;" + std::to_string(it.second.m_SessionId), websocketpp::frame::opcode::text);
		}
	}
	GameLogic::getInstance()->deleteClient(_hdl);
}


//Message(when message comes)
void ConnectionManager::Handle_Message(connection_hdl _hdl, WSServerT::message_ptr _msg)
{
	std::string command = _msg->get_payload().substr(0, _msg->get_payload().find(';'));
	std::string body = _msg->get_payload().substr(_msg->get_payload().find(';') + 1, _msg->get_payload().length());
	std::string message;
	if (command == "connected")
	{

		UserInformation* user = ControllerFileManager::getInstance()->FindWithID(atoi(body.c_str()));

		if (user != nullptr)
		{
			Constant::getInstance()->ReplaceUser(_hdl,user);

		}
	}
	message = parserController->parserPackage(new Package(command, body, message,_hdl));


	//this is the first command, which sets name of client
	/*if (command == "AddUser")
	{
		Constant::getInstance()->getM_Connetcions[_hdl].m_Name = body;

		//Form 
		message = "User Add (ID = " + std::to_string(Constant::getInstance()->getIdForPlayer()) + ").";

		//message = "User \"" + m_Connections[_hdl].m_Name + "\" connected (ID = " + std::to_string(Constant::getInstance()->getIdForPlayer()) + ").";

		//Print message
		std::cout << message << std::endl;

		//Write log
		Util_WriteLog(message);
		std::cout << message << std::endl;
		//Send message to everybody*/
	std::cout <<(l++)<< message << std::endl;
	Util_WriteLog(message);
	//auto ghj = Constant::getInstance()->getM_Connetcions();
	if (message != "")
		for (auto it : Constant::getInstance()->getM_Connetcions())
		{
			 if (command != "hello" &&  Constant::getInstance()->getID(_hdl) != it.second.m_SessionId)
				m_Server.send(it.first, message, websocketpp::frame::opcode::text);
			
		}
	



	//this is command of accepting message
	/*else if (command == "msg")
	{
		//Form message
		message = "[" + Constant::getInstance()->getM_Connetcions[_hdl].m_Name + "](" + std::to_string(Constant::getInstance()->getM_Connetcions[_hdl].m_SessionId) + "):" + body;

		//Print message
		std::cout << message << std::endl;

		//Write log
		Util_WriteLog(message);
		*
		//Send message to everybody*/
	/*	for (auto it : Constant::getInstance()->getM_Connetcions)
		{
			
			m_Server.send(it.first, message, websocketpp::frame::opcode::text);
		}*/
	//}
}