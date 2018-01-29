#include "Character.h"
#include"TileCell.h"

#include "PathfindingldeState.h"


PathfindingIdleState::PathfindingIdleState(Character* character) : State(character)
{
}

PathfindingIdleState::~PathfindingIdleState()
{
}

void PathfindingIdleState::Update(float deltaTime)
{
	State::Update(deltaTime);

	if (eStateType::ST_NONE != _nextState)
	{
		_character->ChangeState(_nextState);
		return;
	}

	TileCell* targetTileCell = _character->GetTargetTileCell();
	if (NULL != targetTileCell)
	{
		_nextState = eStateType::ST_PATHFINDING;
	}
}