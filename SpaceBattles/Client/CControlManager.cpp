#include "CControlManager.h"




// Game manager instance
CControlManager* CControlManager::mControlManager = nullptr;




// Get instance
CControlManager* CControlManager::getInstance()
{
	if (!mControlManager)
		mControlManager = new CControlManager();
	return mControlManager;
}




// Constructor
CControlManager::CControlManager()
{
	// Initially all keys are not pressed
	for (int i = 0; i < 128; i++)
	{
		mKeyState[i] = false;
	}
}




// Destructor
CControlManager::~CControlManager()
{
}




// Get key state
bool CControlManager::isKeyDown(char _key)
{
	return mKeyState[_key];
}




// Set key state
void CControlManager::setKeyState(char _key, bool _state)
{
	mKeyState[_key] = _state;
}