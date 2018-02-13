#include "ComponentSystem.h"
#include "Map.h"
#include "InputSystem.h"
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

	// 마우스 클릭 -> 클릭된 위치의 타일셀을 타겟 타일셀로 세팅
	if (InputSystem::GetInstance()->IsMouseDown())
	{
		int mouseX = InputSystem::GetInstance()->GetMouseX();
		int mouseY = InputSystem::GetInstance()->GetMouseY();

		Map* map = (Map*)ComponentSystem::GetInstance()->FindComponent(L"Map");
		TileCell* tileCell = map->FindTileCellByMousePosition(mouseX, mouseY);

		if (NULL != tileCell)
		{
			_character->SetTargetTileCell(tileCell);
		}
	}

	TileCell* targetTileCell = _character->GetTargetTileCell();
	if (NULL != targetTileCell)
	{
		_nextState = eStateType::ST_PATHFINDING;
	}
}