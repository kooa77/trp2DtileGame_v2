#pragma once

#include <list>
#include "Character.h"

class Monster : public Character
{
public:
	Monster(std::wstring name);
	~Monster();

public:
	void UpdateAI(float deltaTime);

	std::vector<Component*> Collision(std::vector<Component*> collisionList);
};

