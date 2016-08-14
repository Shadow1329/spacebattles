#pragma once
#include "Package.h"
#include "Constant.h"
 class ParserController
{
public:

	ParserController()
	{
		parserNext = nullptr;
	}

	void setNextParserController(ParserController* parserController)
	{
		if (this->parserNext != nullptr)
		{
			this->parserNext->setNextParserController(parserController);
		}
		else
		{
			this->parserNext = parserController;
		}
	}

	std::string parserPackage(Package* package)
	{
		std::string message;
		if (package->getCommand() == mask)
		{
			message = parser(package);
		}
		else if(parserNext != nullptr)
		{
			message = parserNext->parserPackage(package);

		}
		return message;
	}
	virtual std::string parser(Package* package) { return ""; };

private:
	
	ParserController* parserNext;
protected:
	std::string mask;
	
	
};

