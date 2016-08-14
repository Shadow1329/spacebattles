#include "ParserAddNewUser.h"



ParserAddNewUser::ParserAddNewUser() 
{
	mask = "hello";
}


ParserAddNewUser::~ParserAddNewUser()
{
}

std::string ParserAddNewUser::parser(Package* package)
{
	std::string message;
	Constant::getInstance()->setNameUser(package->getHDL(), package->getBody());


	if (!ControllerFileManager::getInstance()->isCheckedNameUser(package->getBody()) && package->getBody() != "")
	{
		//Form message
		Constant::getInstance()->setNameUser(package->getHDL(), package->getBody());
		message = "User \"" + Constant::getInstance()->getM_Connetcions()[package->getHDL()].m_Name + "\" connected (ID = " + std::to_string(Constant::getInstance()->getM_Connetcions()[package->getHDL()].m_SessionId) + ").";
		ControllerFileManager::getInstance()->AddUserName(new UserInformation(Constant::getInstance()->getM_Connetcions()[package->getHDL()].m_SessionId, package->getBody()));

	}else
	{
		message = "User \"\" (ID = " + std::to_string(Constant::getInstance()->getM_Connetcions()[package->getHDL()].m_SessionId) + ").";
	}
	//message = "User Add (ID = " + std::to_string(Constant::getInstance()->getIdForPlayer()) + ").";

	//message = "User \"" + m_Connections[_hdl].m_Name + "\" connected (ID = " + std::to_string(Constant::getInstance()->getIdForPlayer()) + ").";

	return message;
}
