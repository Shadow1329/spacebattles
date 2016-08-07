#pragma once

#include <string>




//Game object class
class CGameObject
{
private:
	int mID;				// ID
	float mX;				// X-coord
	float mY;				// Y-coord
	float mAngle;			// Rotation angle
	int mSize;				// Size
	std::string mName;		// Name
	std::string mTexture;	// Texture



public:
	CGameObject();
	~CGameObject() {};


	//Update game object
	void Update();
};

