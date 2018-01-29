#pragma once

#include "Monster.h"

class PathfinderMonster : public Monster
{
public:
	PathfinderMonster(std::wstring name);
	~PathfinderMonster();

public:
	void Init(std::wstring textureFilename, std::wstring scriptFilename);
	void UpdateAI(float deltaTime);
};
