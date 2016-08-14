#include "ConnectionManager.h"
#include "Constant.h"

#include <cstdlib>
#include <iostream>
#include <map>
#include <string>
#include <sstream>

#include <stdio.h>
#include <stdlib.h>
#include "ControllerFileManager.h"
//#include <unistd.h>


using namespace std;

void main()
{
	ControllerFileManager::getInstance()->getInformationUser();


	//ControllerFileManager::getInstance()->AddUserName(new UserInformation(6, "Nikita"));
	
	string role_switch = "0";

	


		//If it was started as app

			//main menu
		while (role_switch != "3")
		{
			if (system("cls")) system("clear");
			cout << "Welcome to SimpleChat!" << endl;
			cout << "Enter 1 or 2 to select mode, or 3 to exit:" << endl;
			cout << "[1]: Client" << endl;
			cout << "[2]: Server" << endl;
			cout << "[3]: Exit" << endl;

			//Waiting for user input and checking
			while (1)
			{
				getline(cin, role_switch);
				if ((role_switch != "1") && (role_switch != "2") && (role_switch != "3")) cout << "Only 1, 2 or 3" << endl;
				else break;
			}
			if (system("cls")) system("clear");

			//Loading client
			/*if (role_switch == "1")
			{
				ConnectionManager my_client;
				my_client.Start();
			}*/

			//Loading server
			else if (role_switch == "2")
			{
				ConnectionManager my_server;
				my_server.Run();
			}

			//Quit
			else
			{
				cout << "Good Bye!" << endl;
			}
		}
	
}

	



