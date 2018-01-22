#pragma once

#include "State.h"

class DeadState : public State
{
public:
	DeadState(Character* character);
	~DeadState();

public:
	void Start();
	void Update(float deltaTime);

	// 5-1 ¿Á¡§¿«
	void CreateSprite(std::wstring textureFilename, std::wstring scriptFilename);
};
