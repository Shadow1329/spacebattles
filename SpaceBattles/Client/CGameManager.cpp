#include "CGameManager.h"
#include "CGraphicsManager.h"




//Max FPS
#define MAX_FPS 30








// Game manager instance
CGameManager* CGameManager::mGameManager = nullptr;




// Get instance
CGameManager* CGameManager::getInstance()
{
	if (!mGameManager)
		mGameManager = new CGameManager();
	return mGameManager;
}




// Init game manager
void CGameManager::Init()
{
	//Clear objects
	//mGameObjects.clear();

	//Start updating in new process
	mWorking = true;
	//_beginthread(CGameManager::getInstance()->Update, 0, NULL);
	new std::thread(&CGameManager::Update, CGameManager::getInstance());
}




// Release game manager
void CGameManager::Release()
{
	//Clear objects
	mGameObjects.clear();

	//Start updating in new process
	mWorking = false;
}




// Update objects
void CGameManager::Update()
{
	while (mWorking)
	{
		//Get tick count
		DWORD begin_tick = GetTickCount();

		//Put objects from queue
		if (mGameObjectsQueue.size() > 0)
		{
			mGameObjects.push_back(mGameObjectsQueue[0]);
			mGameObjectsQueue.erase(mGameObjectsQueue.begin());
		}

		//Update and draw	
		for (int i = 0; i < mGameObjects.size(); i++)
		{
			mGameObjects[i]->Update();
			CGraphicsManager::getInstance()->AddData(mGameObjects[i]->GetX(), mGameObjects[i]->GetY(), mGameObjects[i]->GetSize());
		}
		CGraphicsManager::getInstance()->UpdateBuffer();
		
		
		//Get tick count
		DWORD end_tick = GetTickCount();
		int sleep_time = (1000 / MAX_FPS) - (end_tick - begin_tick);
		if (sleep_time > 0) Sleep(sleep_time);
	}
}




// Create new object
void CGameManager::CreateObject(int _id) 
{
	CGameObject* new_game_object = new CGameObject();

	new_game_object->SetID(_id);
	new_game_object->SetX(0);
	new_game_object->SetY(0);
	new_game_object->SetSize(1);
	new_game_object->SetController(CGameObject::TYPE_REMOTE);

	mGameObjectsQueue.push_back(new_game_object);
}




// Create player
void CGameManager::CreatePlayer(int _id)
{
	CGameObject* new_game_object = new CGameObject();

	new_game_object->SetID(_id);
	new_game_object->SetX(0);
	new_game_object->SetY(0);
	new_game_object->SetSize(1);
	new_game_object->SetController(CGameObject::TYPE_PLAYER);

	mGameObjectsQueue.push_back(new_game_object);
}




// Delete object
void CGameManager::DeleteObject(int _id)
{
	for (int i = 0; i < mGameObjects.size(); i++)
	{
		if (mGameObjects[i]->GetID() == _id)
		{
			mGameObjects.erase(mGameObjects.begin() + i);
			break;
		}
	}
}
