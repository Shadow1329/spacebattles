#pragma once

#include <string>
#include <vector>

#include "CGameObject.h"




//Game manager class
class CGameManager
{
private:
	std::vector<CGameObject*> mGameObjects;		// Objects


public:
	CGameManager() {};
	~CGameManager() {};

	// Init game manager
	void Init();

	// Update objects
	void Update();

	// Draw objects
	void Draw();

	// Create new object
	void CreateObject(int _id);

	// Delete object
	void DeleteObject(int _id);

	// Update object
	void UpdateObject(int _id, std::string _params);
};

