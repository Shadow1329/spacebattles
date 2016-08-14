#include "ParserUpdateObject.h"
#include "GameLogic.h"


ParserUpdateObject::ParserUpdateObject()
{
	mask = "update_object";
}


ParserUpdateObject::~ParserUpdateObject()
{
}


std::string ParserUpdateObject::parser(Package* package)
{
	std::string message;
	message = "update_object;" + package->getBody();
	GameLogic::getInstance()->updateClient(package->getHDL(), package->getBody());

	return message;
}