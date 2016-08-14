#include "ParserMessage.h"



ParserMessage::ParserMessage()
{
	
	mask = "msg";
}


ParserMessage::~ParserMessage()
{
}


std::string ParserMessage::parser(Package* package)
{
	std::string message; 
	message = "[" + Constant::getInstance()->getM_Connetcions()[package->getHDL()].m_Name + "](" + std::to_string(Constant::getInstance()->getM_Connetcions()[package->getHDL()].m_SessionId) + "):" + package->getBody();

	return message;
}
