#include "ComponentSystem.h"
#include "Map.h"

#include "PathfindingldeState.h"
#include "PathfindingState.h"
#include "PathfindingImmedateState.h"
#include "PathfindingMoveState.h"

#include "PathfinderPlayer.h"


PathfinderPlayer::PathfinderPlayer(std::wstring name) : Player(name)
{
}

PathfinderPlayer::~PathfinderPlayer()
{
}

void PathfinderPlayer::Init(std::wstring textureFilename, std::wstring scriptFilename)
{
	Player::Init(textureFilename, scriptFilename);
	/*
	Map* map = (Map*)ComponentSystem::GetInstance()->FindComponent(L"Map");
	if (NULL != map)
	{
		map->ResetTileComponent(_tilePosition, this);
		_tilePosition.x = 3;
		_tilePosition.y = map->GetHeight() * 0.5;
		map->SetTileComponent(_tilePosition, this);
	}
	*/
}

void PathfinderPlayer::UpdateAI(float deltaTime)
{
}

void PathfinderPlayer::InitState(std::wstring textureFilename, std::wstring scriptFilename)
{
	Player::InitState(textureFilename, scriptFilename);

	// Idle 상태 교체
	{
		State* state = new PathfindingIdleState(this);
		state->CreateSprite(textureFilename, scriptFilename);
		_stateMap[eStateType::ST_IDLE] = state;
	}
	{
		//State* state = new PathfindingState(this);
		State* state = new PathfindingImmedateState(this);
		state->CreateSprite(textureFilename, scriptFilename);
		_stateMap[eStateType::ST_PATHFINDING] = state;
	}
	{
		State* state = new PathfindingMoveState(this);
		state->CreateSprite(textureFilename, scriptFilename);
		_stateMap[eStateType::ST_MOVE] = state;
	}
}
