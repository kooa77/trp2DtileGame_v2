#pragma once

#include "PathfindingState.h"

class PathfindingImmedateState : public PathfindingState
{
public:
	PathfindingImmedateState(Character* character);
	~PathfindingImmedateState();

public:
	void Start();
};