#include "ControllerFileManager.h"

ControllerFileManager* ControllerFileManager::controllerFileManager = nullptr;

ControllerFileManager::ControllerFileManager()
{

	
	
}


ControllerFileManager::~ControllerFileManager()
{
}


ControllerFileManager* ControllerFileManager::getInstance()
{
	if (!controllerFileManager)
	{
		controllerFileManager = new ControllerFileManager;
	}

	return controllerFileManager;
}


void ControllerFileManager::AddUserName(UserInformation* userInformation)
{
	fout.open("C:\\Users\\user\\spacebattles\\spacebattles\\SpaceBattles\\Server\\Debug\\db.txt", ios_base::app);

	fout << userInformation->getUserID() << " " << userInformation->getUserName() << endl;
	fout.close();
	ControllerFileManager::getInstance()->getInformationUser();
}

void ControllerFileManager::getInformationUser()
{
	fin.open("C:\\Users\\user\\spacebattles\\spacebattles\\SpaceBattles\\Server\\Debug\\db.txt");
	
	stringstream ss;
	
	
	
	
	char buff[50];
	while (true)
	{
		fin >> buff;
		if (buff[0] == '\0')
		{
			break;
		}
		
		int id = atoi(buff);
		fin >> buff;
		ss << buff;
		string name;
		ss >> name;
		usersInformation.push_back(new UserInformation(id, name));

		memset(&buff[0], 0, sizeof(buff));
		
	    
	}

	fin.close();
	

}


bool ControllerFileManager::isCheckedNameUser(std::string name)
{
	for (auto it : usersInformation)
	{
		if (it->getUserName() == name)
		{
			return true;
		}
	}

	return false;
}



