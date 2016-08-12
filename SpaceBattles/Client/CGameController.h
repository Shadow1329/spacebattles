#pragma once


//Game controller
class CGameController
{
public:

	// Constructor and destructor
	CGameController(){}
	~CGameController(){}

	// Control
	virtual void Control(void* _data) {};
};




//Player controller
class CGameControllerPlayer : public CGameController
{
public:

	// Constructor and destructor
	CGameControllerPlayer();
	~CGameControllerPlayer();

	// Control
	void Control(void* _data);
};




//Player controller
class CGameControllerRemote : public CGameController
{
public:

	// Constructor and destructor
	CGameControllerRemote();
	~CGameControllerRemote();

	// Control
	void Control(void* _data);
};
