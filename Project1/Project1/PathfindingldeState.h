#pragma once

#include "State.h"

class PathfindingIdleState : public State
{
public:
	PathfindingIdleState(Character* character);
	~PathfindingIdleState();

public:
	void Update(float deltaTime);
};
