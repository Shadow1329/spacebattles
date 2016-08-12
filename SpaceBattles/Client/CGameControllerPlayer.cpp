#include "CGameController.h"
#include "CGameObject.h"
#include "CControlManager.h"
#include "CConnectionManager.h"




// Constructor
CGameControllerPlayer::CGameControllerPlayer()
{
}




// Destructor
CGameControllerPlayer::~CGameControllerPlayer()
{
}




// Control
void CGameControllerPlayer::Control(void* _data)
{
	CGameObject *obj = static_cast<CGameObject*>(_data);

	// Get control
	if (CControlManager::getInstance()->isKeyDown(87)) 
		obj->SetY(obj->GetY() + obj->GetSpeed());
	else if (CControlManager::getInstance()->isKeyDown(83))
		obj->SetY(obj->GetY() - obj->GetSpeed());
	else if (CControlManager::getInstance()->isKeyDown(68))
		obj->SetX(obj->GetX() + obj->GetSpeed());
	else if (CControlManager::getInstance()->isKeyDown(65))
		obj->SetX(obj->GetX() - obj->GetSpeed());

	std::string message = "update_object;" +
		                  std::to_string(obj->GetID()) + ";" +
						  std::to_string(obj->GetX()) + ";" + 
						  std::to_string(obj->GetY());
	CConnectionManager::getInstance()->Send(message);
}