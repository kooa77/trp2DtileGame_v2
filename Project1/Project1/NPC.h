#pragma once

#include "Character.h"

class NPC : public Character
{
public:
	NPC(std::wstring name);
	~NPC();
	
public:
	void UpdateAI(float deltaTime);
	std::vector<Component*> Collision(std::vector<Component*> collisionList);
};
