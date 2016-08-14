#include "ParserCreateObject.h"



ParserCreateObject::ParserCreateObject()
{
	mask = "create_object";
}


ParserCreateObject::~ParserCreateObject()
{
}


std::string ParserCreateObject::parser(Package* package)
{
	std::string message;
	message = "[" + Constant::getInstance()->getM_Connetcions()[package->getHDL()].m_Name + "](" + std::to_string(Constant::getInstance()->getM_Connetcions()[package->getHDL()].m_SessionId) + "):" + package->getBody();
	return message;
}