#pragma once
#include "ParserController.h"
class ParserCreateObject :
	public ParserController
{
public:
	ParserCreateObject();
	~ParserCreateObject();

	std::string parser(Package* package);
};

