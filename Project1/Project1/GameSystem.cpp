#include "GameSystem.h"

GameSystem* GameSystem::_instance = 0;

GameSystem* GameSystem::GetInstance()
{
	if (0 == _instance)
	{
		_instance = new GameSystem();
	}
	return _instance;
}