#pragma once

#include "Character.h"

class Player : public Character
{
public:
	Player(std::wstring name);
	~Player();

public:
	void UpdateAI(float deltaTime);
	std::vector<Component*> Collision(std::vector<Component*> collisionList);
};
