#pragma once


// Control manager
class CControlManager
{
private:
	//Instanse
	static CControlManager *mControlManager;	// Instance

	//Key states
	bool mKeyState[128];

	


public:
	// Get instance
	static CControlManager* getInstance();

	// Constructor and destructor
	CControlManager();
	~CControlManager();

	// Get key state
	bool isKeyDown(char _key);

	// Set key state
	void setKeyState(char _key, bool _down);
};

