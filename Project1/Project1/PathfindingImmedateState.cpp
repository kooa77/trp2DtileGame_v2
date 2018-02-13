#include "Character.h"
#include "PathfindingImmedateState.h"

PathfindingImmedateState::PathfindingImmedateState(Character* character) : PathfindingState(character)
{

}

PathfindingImmedateState::~PathfindingImmedateState()
{

}

void PathfindingImmedateState::Start()
{
	PathfindingState::Start();

	while (0 != _pathfindingQueue.size())
	{
		if (eUpdateState::BUILD_PATH == _updateState)
			break;
		UpdatePathfinding();
	}

	while (eStateType::ST_MOVE != _nextState)
	{
		UpdateBuildPath();
	}
}