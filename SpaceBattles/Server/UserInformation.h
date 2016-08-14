#pragma once

#include <string>
#include <stdio.h>
#include <iostream>

class UserInformation
{
private:
	int ID;
	std::string Name;

public:
	UserInformation(int ID, std::string Name)
	{
		this->ID = ID;
		this->Name = Name;
	}

	int getUserID() { return ID; };
	std::string getUserName() { return Name; };



};