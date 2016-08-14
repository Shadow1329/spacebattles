#include "CGameObject.h"




// Constructor
CGameObject::CGameObject()
{
	mID = -1;
	mX = 0;
	mY = 0;
	mAngle = 0;
	mSize = 0;
	mSpeed = 0.05;
	mName = "";
	mTexture = "";
}




//ID
int CGameObject::GetID()
{
	return mID;
}


void CGameObject::SetID(int _id)
{
	mID = _id;
}




// X
float CGameObject::GetX()
{
	return mX;
}


void CGameObject::SetX(float _x)
{
	mX = _x;
}




// Y
float CGameObject::GetY()
{
	return mY;
}


void CGameObject::SetY(float _y)
{
	mY = _y;
}




// Size
int CGameObject::GetSize()
{
	return mSize;
}


void CGameObject::SetSize(int _size)
{
	mSize = _size;
}




// Speed
float CGameObject::GetSpeed()
{
	return mSpeed;
}




// Controller
void CGameObject::SetController(int _type)
{
	if (_type == TYPE_PLAYER)
	{
		mController = new CGameControllerPlayer();
	}
	else if (_type == TYPE_REMOTE)
	{
		mController = new CGameControllerRemote();
	}
}




// Update game object
void CGameObject::Update()
{
	mController->Control(this);
}