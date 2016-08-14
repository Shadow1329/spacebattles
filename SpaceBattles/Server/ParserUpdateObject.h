#pragma once
#include "ParserController.h"

class ParserUpdateObject : public ParserController
{
public:
	ParserUpdateObject();
	~ParserUpdateObject();

	std::string parser(Package* package);
};

