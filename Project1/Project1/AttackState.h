#pragma once

#include "State.h"

class AttackState : public State
{
public:
	AttackState(Character* character);
	~AttackState();

public:
	void Start();
	void Update(float deltaTime);
};
