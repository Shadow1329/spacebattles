#include "CConnectionManager.h"
#include "CGameManager.h"




// Connection manager instance
CConnectionManager* CConnectionManager::mConnectionManager = nullptr;




// Get instance
CConnectionManager* CConnectionManager::getInstance()
{
	if (!mConnectionManager)
		mConnectionManager = new CConnectionManager();
	return mConnectionManager;
}




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
void CConnectionManager::Init()
{
	//Clear queue
	m_Queue.clear();

	//Set address
	m_Address = "7.112.201.67:9002";

	//Connect to server
	Control_Connect("ws://" + m_Address + "/");
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
void CConnectionManager::Send(std::string const & _msg)
{
	try
	{
		//Try to send message
		m_Client.send(m_Hdl, _msg, websocketpp::frame::opcode::text);
	}
	catch (websocketpp::exception const & e)
	{
		//Error
		std::cout << "Invalid message." << std::endl;
	}
}




//Get data
std::string CConnectionManager::GetData(int _id)
{
	std::string data = "none";
	for (int i = 0; i < m_Queue.size(); i++)
	{
		if (m_Queue[i].mID == _id)
		{	
			data = m_Queue[i].mData;
			m_Queue.erase(m_Queue.begin() + i);
			break;
		}
	}
	return data;
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
	//Write log
	Util_WriteLog("Successfully connected to \"" + m_Address + "\".");

	//Create game manager
	CGameManager::getInstance()->Init();

	try
	{
		//Send name to server
		m_Client.send(_hdl, "hello;", websocketpp::frame::opcode::text);

		//Set state of connection to true
		m_Connection_Succsesful = true;
	}
	catch (websocketpp::exception const & e)
	{
		//If name is incorrect - close connection
		m_Client.close(m_Hdl, websocketpp::close::status::going_away, "");
	}
}




//Close(if client disconnect from server)
void CConnectionManager::onClose(connection_hdl _hdl)
{
	//Set state of connection to false
	m_Connection_Succsesful = false;

	//Write log
	Util_WriteLog("Disconnected from \"" + m_Address + "\".");
}




//Close(if connection was failed)
void CConnectionManager::onFail(connection_hdl _hdl)
{
	//Set state of connection to false
	m_Connection_Succsesful = false;

	//Write log
	Util_WriteLog("Failed to connect to \"" + m_Address + "\".");
}




//Message(when message recieved)
void CConnectionManager::onMessage(connection_hdl _hdl, WSClientT::message_ptr _msg)
{
	//Write log
	Util_WriteLog(_msg->get_payload());

	int id = 0;
	std::string message = _msg->get_payload();
	std::string command = message.substr(0, message.find(';'));
	message = message.substr(message.find(';') + 1, message.length());

	if (command == "connection_accepted")
	{
		id = atoi(message.substr(0, message.length()).c_str());
		CGameManager::getInstance()->CreatePlayer(id);
	}
	else if (command == "create_object")
	{
		id = atoi(message.substr(0, message.length()).c_str());
		CGameManager::getInstance()->CreateObject(id);
	}
	else if (command == "update_object")
	{
		id = atoi(message.substr(0, message.find(';')).c_str());
		message = message.substr(message.find(';') + 1, message.length());

		CConnectionData data;
		data.mID = id;
		data.mData = message;
		m_Queue.push_back(data);
		if (m_Queue.size() > 20)
			m_Queue.erase(m_Queue.begin());
	}
}