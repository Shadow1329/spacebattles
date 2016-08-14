#include "CGameController.h"
#include "CGameObject.h"
#include "CConnectionManager.h"




// Constructor
CGameControllerRemote::CGameControllerRemote()
{
}




// Destructor
CGameControllerRemote::~CGameControllerRemote()
{
}




// Control
void CGameControllerRemote::Control(void* _data)
{
	CGameObject *obj = static_cast<CGameObject*>(_data);

	float x, y;
	std::string message = CConnectionManager::getInstance()->GetData(obj->GetID());

	if (message != "none")
	{
		x = atof(message.substr(0, message.find(';')).c_str());
		message = message.substr(message.find(';') + 1, message.length());

		y = atof(message.substr(0, message.length()).c_str());

		obj->SetX(x);
		obj->SetY(y);
	}
}