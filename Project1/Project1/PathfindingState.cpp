#include "ComponentSystem.h"
#include "Map.h"
#include "TileCell.h"
#include "NPC.h"
#include "Player.h"

#include "Character.h"
#include "PathfindingState.h"

PathfindingState::PathfindingState(Character* character) : State(character)
{
}

PathfindingState::~PathfindingState()
{
}

void PathfindingState::Start()
{
	State::Start();

	_map = (Map*)ComponentSystem::GetInstance()->FindComponent(L"Map");
	_map->ResetPahtfinding();

	_targetTileCell = _character->GetTargetTileCell();

	TileCell* startTileCell = _map->GetTileCell(_character->GetTilePosition());

	sPathCommand command;
	command.tileCell = startTileCell;
	command.heuristic = 0.0f;
	_pathfindingQueue.push(command);

	_updateState = eUpdateState::PATHFINDING;
}

void PathfindingState::Stop()
{
	State::Stop();

	while (0 != _pathfindingQueue.size())
	{
		_pathfindingQueue.pop();
	}
}

void PathfindingState::Update(float deltaTime)
{
	State::Update(deltaTime);

	if (eStateType::ST_NONE != _nextState)
	{
		_character->ChangeState(_nextState);
		return;
	}

	switch (_updateState)
	{
	case eUpdateState::PATHFINDING:
		UpdatePathfinding();
		break;
	case eUpdateState::BUILD_PATH:
		UpdateBuildPath();
		break;
	}
}

void PathfindingState::UpdatePathfinding()
{
	// 길찾기 업데이트 시작
	if (0 != _pathfindingQueue.size())	// 큐에 무언가가 들어있으면 맨 앞에 원소를 빼서 검사
	{
		sPathCommand command = _pathfindingQueue.top();	// 맨 앞에 있는 원소를 빼냄
		_pathfindingQueue.pop();	// 빼낸 후 큐에서 제거

		if (false == command.tileCell->IsSearchPathfinding())
		{
			// 아직 검사하지 않은 타일 셀이면
			command.tileCell->SearchPathfind();	// 검사 했다고 체크

												// 목표에 도달 했는가?
			if (command.tileCell->GetTilePosition().x == _targetTileCell->GetTilePosition().x &&
				command.tileCell->GetTilePosition().y == _targetTileCell->GetTilePosition().y)
			{
				//_nextState = eStateType::ST_MOVE;
				_reverseTileCell = command.tileCell;
				_updateState = eUpdateState::BUILD_PATH;
				return;
			}

			for (int direction = 0; direction < (int)eDirection::NONE; direction++)
			{
				TilePoint currentTilePosition = command.tileCell->GetTilePosition();
				TilePoint searchTilePosision = GetSearchTilePositionByDirection(currentTilePosition, (eDirection)direction);
				TileCell*  searchTileCell = _map->GetTileCell(searchTilePosision);

				if (NULL != searchTileCell &&
					false == searchTileCell->IsSearchPathfinding() &&
					(true == searchTileCell->CanMove() ||
					(searchTileCell->GetTilePosition().x == _targetTileCell->GetTilePosition().x &&
						searchTileCell->GetTilePosition().y == _targetTileCell->GetTilePosition().y)
						)
					)
				{
					float distanceFromStart = command.tileCell->GetDistanceFromStart() + 1;
					float heuristic = distanceFromStart;
					
					if (NULL == searchTileCell->GetPrevTileCell())
					{
						// 큐에 넣는다.
						searchTileCell->SetPrevTileCell(command.tileCell);
						searchTileCell->SetDistanceFromStart(distanceFromStart);

						sPathCommand newCommand;
						newCommand.tileCell = searchTileCell;
						newCommand.heuristic = heuristic;
						_pathfindingQueue.push(newCommand);

						CheckTestMark(searchTileCell);
					}
				}
			}
		}
	}
}

void PathfindingState::UpdateBuildPath()
{
	// 목표 지점까지 경로 구축
	// 거꾸로 돌아가면서 길을 도출
	if (NULL != _reverseTileCell)
	{
		CheckBuildTestMark(_reverseTileCell);
		_reverseTileCell = _reverseTileCell->GetPrevTileCell();
	}
}

TilePoint PathfindingState::GetSearchTilePositionByDirection(TilePoint tilePosition, eDirection direction)
{
	TilePoint searchTilePosition = tilePosition;
	switch (direction)
	{
	case eDirection::LEFT:
		searchTilePosition.x--;
		break;
	case eDirection::RIGHT:
		searchTilePosition.x++;
		break;
	case eDirection::UP:
		searchTilePosition.y--;
		break;
	case eDirection::DOWN:
		searchTilePosition.y++;
		break;
	}
	return searchTilePosition;
}

void PathfindingState::CheckTestMark(TileCell* tileCell)
{
	NPC* npc = new NPC(L"testmark");
	npc->Init(L"character_sprite_pack.png", L"npc");
	npc->MoveStart(tileCell->GetTilePosition());
	npc->MoveStop();

	// 캐릭터가 자기 이전타일을 바라보도록 방향 설정
	TilePoint curPosition = tileCell->GetTilePosition();
	TilePoint prevPosition = tileCell->GetPrevTileCell()->GetTilePosition();
	eDirection direction;
	if (prevPosition.x < curPosition.x)
		direction = eDirection::LEFT;
	if (curPosition.x < prevPosition.x)
		direction = eDirection::RIGHT;
	if (prevPosition.y < curPosition.y)
		direction = eDirection::UP;
	if(curPosition.y < prevPosition.y)
		direction = eDirection::DOWN;
	npc->SetDirection(direction);
}

void PathfindingState::CheckBuildTestMark(TileCell* tileCell)
{
	Player* player = new Player(L"testmark");
	player->Init(L"character_sprite.png", L"player");
	player->MoveStart(tileCell->GetTilePosition());
	player->MoveStop();

	// 캐릭터가 자기 이전타일을 바라보도록 방향 설정
	if (NULL != tileCell->GetPrevTileCell())
	{
		TilePoint curPosition = tileCell->GetTilePosition();
		TilePoint prevPosition = tileCell->GetPrevTileCell()->GetTilePosition();
		eDirection direction;
		if (prevPosition.x < curPosition.x)
			direction = eDirection::RIGHT;
		if (curPosition.x < prevPosition.x)
			direction = eDirection::LEFT;
		if (prevPosition.y < curPosition.y)
			direction = eDirection::DOWN;
		if (curPosition.y < prevPosition.y)
			direction = eDirection::UP;
		player->SetDirection(direction);
	}
}
