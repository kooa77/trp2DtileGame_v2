#pragma once

#include "State.h"

class Character;

class PathfindingMoveState : public State
{
public:
	PathfindingMoveState(Character* character);
	~PathfindingMoveState();

private:
	float _movingDuration;

public:
	void Start();
	void Stop();
	void Update(float deltaTime);

	eDirection GetDirection(TilePoint toPostion, TilePoint fromPosition);
};
