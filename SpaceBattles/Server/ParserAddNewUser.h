#pragma once
#include "ParserController.h"
#include "ControllerFileManager.h"
class ParserAddNewUser: public ParserController
{
public:
	ParserAddNewUser();
	~ParserAddNewUser();

	std::string parser(Package* package);
};

