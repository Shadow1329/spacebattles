#include "CConnectionManager.h"




//Constuctor
CConnectionManager::CConnectionManager()
{
	//State of connection
	m_Connection_Succsesful = false;

	//Create default address
	m_Address = "";

	// Configure log messages
	m_Client.clear_access_channels(websocketpp::log::alevel::all);
	m_Client.clear_error_channels(websocketpp::log::elevel::all);

	// Initialize ASIO
	m_Client.init_asio();

	//Start point of thread
	m_Client.start_perpetual();

	// Register our handlers
	m_Client.set_open_handler(bind(&CConnectionManager::onOpen, this, ::_1));
	m_Client.set_close_handler(bind(&CConnectionManager::onClose, this, ::_1));
	m_Client.set_message_handler(bind(&CConnectionManager::onMessage, this, ::_1, ::_2));
	m_Client.set_fail_handler(bind(&CConnectionManager::onFail, this, ::_1));

	// Creating thread
	m_Thread = websocketpp::lib::make_shared<websocketpp::lib::thread>(&WSClientT::run, &m_Client);
}




//Destructor
CConnectionManager::~CConnectionManager()
{
	//End point of thread
	m_Client.stop_perpetual();

	websocketpp::lib::error_code ec;
	m_Client.close(m_Hdl, websocketpp::close::status::going_away, "", ec);

	m_Thread->join();
}




//Start of the client
void CConnectionManager::Start()
{
	std::string filename = "client_data.txt";
	std::string buf = "";
	bool change = false;

	std::cout << "Client mode on" << std::endl;

	//Load user data from file
	std::ifstream input_f(filename);

	//If file not exists
	if (input_f.fail())
	{
		//Ask user to address of server
		while (1)
		{
			std::cout << "Enter server address ((domain name or ip):port): ";
			std::getline(std::cin, m_Address);
			if (m_Address != "") break;
		}

		//Write data to file
		std::ofstream fout(filename);
		fout << m_Address << std::endl;
		fout.close();
	}

	//If file exists
	else
	{
		//Read data
		input_f >> m_Address;
		input_f.close();

		//Make sure that address of server is right
		std::cout << "Server address is \"" << m_Address << "\"?" << std::endl;
		std::cout << "(press Enter to confirm or write new address)" << std::endl;
		std::getline(std::cin, buf);
		if (buf != "")
		{
			m_Address = buf;
			change = true;
		}

		//Write new data to file if smth changed
		if (change == true)
		{
			std::ofstream fout(filename);
			fout << m_Address << std::endl;
			fout.close();
		}
	}

	//Connect to server
	Control_Connect("ws://" + m_Address + "/");

	//Start to chat
	Control_Chat();
}




//Connection to server
void CConnectionManager::Control_Connect(std::string const & _uri)
{
	websocketpp::lib::error_code ec;

	//Get connection
	WSClientT::connection_ptr con = m_Client.get_connection(_uri, ec);

	//Write log
	Util_WriteLog("Try to connect to \"" + m_Address + "\".");

	//If there is error
	if (ec) return;

	//Grab handle of connection
	m_Hdl = con->get_handle();

	//Connect
	m_Client.connect(con);
}




//Disconnect from server
void CConnectionManager::Control_Disconnect()
{
	//Close connection
	m_Client.close(m_Hdl, websocketpp::close::status::going_away, "");
}




//Send message
void CConnectionManager::Control_Send(std::string const & _msg)
{
	try
	{
		//Try to send message
		m_Client.send(m_Hdl, "msg;" + _msg, websocketpp::frame::opcode::text);
	}
	catch (websocketpp::exception const & e)
	{
		//Error
		std::cout << "Invalid message." << std::endl;
	}
}




//Control of the client
void CConnectionManager::Control_Chat()
{
	std::string input;
	while (1)
	{
		//Wait for user input
		std::getline(std::cin, input);

		if (m_Connection_Succsesful != false)
		{
			//If the command is /quit
			if (input == "/quit")
			{
				//Disconnect from server
				Control_Disconnect();
			}

			//If the command is /help
			else if (input == "/help")
			{
				//Show all commans
				std::cout << "Commands:" << std::endl;
				std::cout << "/quit - disconnect from server and go to main menu" << std::endl;
				std::cout << "/help - help with commands:" << std::endl;
				std::cout << "Other text, that you write - is a message." << std::endl;
			}

			//If it is message
			else if (input != "")
			{
				//Send message
				Control_Send(input);
			}
		}
		else break;
	}
}




//Write log
void CConnectionManager::Util_WriteLog(std::string _msg)
{
	std::string filename = "client_log.txt";

	//Open log file
	std::ofstream fout(filename, std::ios_base::app);

	//Write string
	fout << "[" << boost::posix_time::second_clock::local_time() << "]: " << _msg << std::endl;

	//Close file
	fout.close();
}




//Open(if client succesfully connect to server)
void CConnectionManager::onOpen(connection_hdl _hdl)
{
	std::cout << "Connection established!" << std::endl;

	//Write log
	Util_WriteLog("Successfully connected to \"" + m_Address + "\".");

	try
	{
		std::cout << "You can write /help to show all command." << std::endl;

		//Send name to server
		m_Client.send(_hdl, "hello;", websocketpp::frame::opcode::text);

		//Set state of connection to true
		m_Connection_Succsesful = true;
	}
	catch (websocketpp::exception const & e)
	{
		//If name is incorrect - close connection
		std::cout << "Invalid name. Change name and try again." << std::endl;
		m_Client.close(m_Hdl, websocketpp::close::status::going_away, "");
	}
}




//Close(if client disconnect from server)
void CConnectionManager::onClose(connection_hdl _hdl)
{
	std::cout << "Closing connection" << std::endl;

	//Set state of connection to false
	m_Connection_Succsesful = false;

	//Write log
	Util_WriteLog("Disconnected from \"" + m_Address + "\".");
}




//Close(if connection was failed)
void CConnectionManager::onFail(connection_hdl _hdl)
{
	std::cout << "Connection failed!" << std::endl;

	//Set state of connection to false
	m_Connection_Succsesful = false;

	//Write log
	Util_WriteLog("Failed to connect to \"" + m_Address + "\".");
}




//Message(when message recieved)
void CConnectionManager::onMessage(connection_hdl _hdl, WSClientT::message_ptr _msg)
{
	//Show message
	std::cout << _msg->get_payload() << std::endl;

	//Write log
	Util_WriteLog(_msg->get_payload());
}