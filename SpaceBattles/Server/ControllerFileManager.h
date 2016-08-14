#pragma once

#include "UserInformation.h"
#include <fstream>
#include <iostream>
#include <vector>
#include <sstream>
#include <string>



using namespace std;


class ControllerFileManager
{

private:
	ifstream fin;
	ofstream fout;
	std::vector<UserInformation*> usersInformation;
	static ControllerFileManager* controllerFileManager;

	void OpenFile();
	void ReadFile();

public:
	ControllerFileManager();
	~ControllerFileManager();

	static ControllerFileManager* getInstance();

	void getInformationUser();

	void AddUserName(UserInformation* userInformation);
	void Close();

	bool isCheckedNameUser(std::string name);


	UserInformation* FindWithID(int id)
	{
		for (auto it : usersInformation)
		{
			if (it->getUserID() == id)
			{
				return it;
			}
		}

		return nullptr;
	}

};

