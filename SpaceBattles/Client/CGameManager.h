#pragma once

#include <string>
#include <vector>
#include <process.h>
#include <thread>
#include <iostream>

#include "CGameObject.h"




//Game manager class
class CGameManager
{
private:
	static CGameManager *mGameManager;				// Instance
	std::vector<CGameObject*> mGameObjects;			// Objects
	std::vector<CGameObject*> mGameObjectsQueue;	// Objects queue
	bool mWorking;									// Working updates


	// Update objects
	void Update();




public:
	// Instance
	static CGameManager* getInstance();

	// Constructor && Destructor
	CGameManager() {};
	~CGameManager() {};

	// Init game manager
	void Init();

	// Release game manager
	void Release();

	// Create player
	void CreatePlayer(int _id);

	// Create new object
	void CreateObject(int _id);

	// Delete object
	void DeleteObject(int _id);
};

