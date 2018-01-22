#pragma once

#include "State.h"

class MoveState : public State
{
public:
	MoveState(Character* character);
	~MoveState();

private:
	float _movingDuration;

public:
	void Start();
	void Stop();
	void Update(float deltaTime);

	void UpdateMove(float deltaTime);
	float GetMovingDuration() { return _movingDuration; }
};
