#include "ComponentSystem.h"
#include "Map.h"
#include "Character.h"

#include "MoveState.h"

MoveState::MoveState(Character* character) : State(character)
{
	_movingDuration = 0.0f;
}

MoveState::~MoveState()
{

}

void MoveState::Start()
{
	State::Start();

	Map* map = (Map*)ComponentSystem::GetInstance()->FindComponent(L"Map");
	if (NULL != map)
	{
		TilePoint newTilePosition = _character->GetTilePosition();
		
		switch (_character->GetNextDirection())
		{
		case eDirection::LEFT:	newTilePosition.x--; break;
		case eDirection::RIGHT:	newTilePosition.x++; break;
		case eDirection::UP:	newTilePosition.y--; break;
		case eDirection::DOWN:	newTilePosition.y++; break;
		}

		if (eDirection::NONE != _character->GetNextDirection())
		{
			_character->SetDirection(_character->GetNextDirection());
		}

		if (newTilePosition.x != _character->GetTilePosition().x || newTilePosition.y != _character->GetTilePosition().y)
		{
			std::vector<Component*> collisionList = map->GetTileCollisionList(newTilePosition);
			if (0 < collisionList.size())
			{
				std::vector<Component*> enemyList = _character->Collision(collisionList);
				if (0 < enemyList.size() && _character->IsAttackCooltime())
				{
					_character->ResetAttackCooltime();
					_character->SetTarget(enemyList);
					_nextState = eStateType::ST_ATTACK;
				}
				else
				{
					_nextState = eStateType::ST_IDLE;
				}
			}
			else
			{
				if (map->CanMoveTile(newTilePosition))
				{
					_character->MoveStart(newTilePosition);
				}
				else
				{
					_nextState = eStateType::ST_IDLE;
				}
			}
		}
		else
		{
			_nextState = eStateType::ST_IDLE;
		}
	}
}

void MoveState::Stop()
{
	_movingDuration = 0.0f;
	_character->MoveStop();
}

void MoveState::Update(float deltaTime)
{
	State::Update(deltaTime);

	if (eStateType::ST_NONE != _nextState)
	{
		_character->ChangeState(_nextState);
		return;
	}

	UpdateMove(deltaTime);
}

void MoveState::UpdateMove(float deltaTime)
{
	if (false == _character->IsLive())
		return;

	if (false == _character->IsMoving())
		return;

	if (_character->GetMoveTime() <= GetMovingDuration())
	{
		_nextState = eStateType::ST_IDLE;
	}
	else
	{
		_movingDuration += deltaTime;
	}
}