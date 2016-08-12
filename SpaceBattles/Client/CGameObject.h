#pragma once

#include <string>

#include "CGameController.h"




//Game object class
class CGameObject
{
private:
	int mID;				// ID
	float mX;				// X-coord
	float mY;				// Y-coord
	float mAngle;			// Rotation angle
	int mSize;				// Size
	float mSpeed;			// Speed
	std::string mName;		// Name
	std::string mTexture;	// Texture

	CGameController* mController;



public:

	// ==== Constants ====
	static const int TYPE_PLAYER = 0;
	static const int TYPE_REMOTE = 1;


	// ==== Functions ====

	// Constructor and destructor
	CGameObject();
	~CGameObject() {};

	// ID
	int GetID();
	void SetID(int _id);

	// X
	float GetX();
	void SetX(float _x);

	// Y
	float GetY();
	void SetY(float _y);

	// Size
	int GetSize();
	void SetSize(int _size);

	// Speed
	float GetSpeed();

	// Controller
	void SetController(int _type);

	// Update game object
	void Update();
};

