#pragma once
#include "ParserController.h"
class ParserMessage: public ParserController
{
public:
	ParserMessage();
	~ParserMessage();

	 std::string parser(Package* package);
};

