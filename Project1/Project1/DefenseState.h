#pragma once

#include "State.h"

class DefenseState : public State
{
public:
	DefenseState(Character* character);
	~DefenseState();

public:
	void Start();
	void Update(float deltaTime);
};

