#include "GameLogic.h"


GameLogic* GameLogic::gameLogic = nullptr;

GameLogic::GameLogic()
{
}


GameLogic::~GameLogic()
{
}


GameLogic* GameLogic::getInstance()
{
	if (!gameLogic)
	{
		gameLogic = new GameLogic();
	}
	return gameLogic;
}

