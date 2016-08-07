#include "CGameManager.h"




// Init game manager
void CGameManager::Init()
{
	//Clear objects
	mGameObjects.clear();
}




// Update objects
void CGameManager::Update()
{
	for (int i = 0; i < mGameObjects.size(); i++)
	{
		mGameObjects[i]->Update();
	}
}




// Draw objects
void CGameManager::Draw()
{

}




// Create new object
void CGameManager::CreateObject(int _id) 
{

}




// Delete object
void CGameManager::DeleteObject(int _id)
{

}




// Update object
void CGameManager::UpdateObject(int _id, std::string _params)
{

}
