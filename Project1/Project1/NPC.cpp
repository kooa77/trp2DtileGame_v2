#include "ComponentSystem.h"
#include "Map.h"
#include "MoveState.h"
#include "IdleState.h"

#include "NPC.h"

NPC::NPC(std::wstring name) : Character(name)
{
	_type = eComponentType::CT_NPC;
}

NPC::~NPC()
{

}

void NPC::UpdateAI(float deltaTime)
{
	if (false == _isLive)
		return;

	if (false == _isMoving)
	{
		Map* map = (Map*)ComponentSystem::GetInstance()->FindComponent(L"Map");
		if (NULL != map)
		{
			// 탐색 - 일정 범위 내에 적이 있는가?
			std::vector<eComponentType> findTypeList;
			findTypeList.push_back(eComponentType::CT_MONSTER);
			Component* findComponent = map->FindComponentInRange(this, 2, findTypeList);

			// 도망
			_nextDirection = eDirection::NONE;
			if (NULL != findComponent)
			{
				TilePoint targetPosition = findComponent->GetTilePosition();
				if (targetPosition.y < _tilePosition.y)
					_nextDirection = eDirection::DOWN;
				else if (_tilePosition.y < targetPosition.y)
					_nextDirection = eDirection::UP;
				else if (targetPosition.x < _tilePosition.x)
					_nextDirection = eDirection::RIGHT;
				else if (_tilePosition.x < targetPosition.x)
					_nextDirection = eDirection::LEFT;
				
				// 갈 수 없는 방향이면 재조정
				TilePoint nextPosition = _tilePosition;
				for (int i = 0; i < 4; i++)
				{
					switch (_nextDirection)
					{
					case eDirection::LEFT:
						nextPosition.x = _tilePosition.x - 1;
						break;
					case eDirection::RIGHT:
						nextPosition.x = _tilePosition.x + 1;
						break;
					case eDirection::UP:
						nextPosition.y = _tilePosition.y - 1;
						break;
					case eDirection::DOWN:
						nextPosition.y = _tilePosition.y + 1;
						break;
					}

					if (map->CanMoveTile(nextPosition))
						break;

					_nextDirection = (eDirection)(((int)_nextDirection + 1) % (int)eDirection::NONE);
				}
			}
			else
			{
				_nextDirection = (eDirection)(rand() % 4);
			}

			if (eDirection::NONE != _nextDirection)
			{
				_state->ChangeState(eStateType::ST_MOVE);
			}
		}
	}
}

std::vector<Component*> NPC::Collision(std::vector<Component*> collisionList)
{
	std::vector<Component*> filterList;
	for (int i = 0; i < collisionList.size(); i++)
	{
		if (eComponentType::CT_MONSTER == collisionList[i]->GetType())
		{
			filterList.push_back(collisionList[i]);
		}
	}
	return filterList;
}
