#include "ComponentSystem.h"
#include "Map.h"
#include "Sprite.h"
#include "Font.h"
#include "MoveState.h"
#include "IdleState.h"
#include "AttackState.h"
#include "DefenseState.h"
#include "DeadState.h"

#include "Character.h"


Character::Character(std::wstring name) : Component(name)
{
	_position.x = _position.y = 0.0f;

	_state = NULL;
	_moveTime = (float)(rand() % 100 + 50) / 100.0f;
	_isMoving = false;

	_currentDirection = eDirection::LEFT;

	_hp = 100;
	_attackPoint = 10;

	_attackCooltime = 1.0f;
	_attackCooltimeDuration = 0.0f;;

	SetCanMove(false);
}

Character::~Character()
{
}

void Character::Init(std::wstring textureFilename, std::wstring scriptFilename)
{
	// 타일인덱스를통한 위치 세팅 테스트
	{
		Map* map = (Map*)ComponentSystem::GetInstance()->FindComponent(L"Map");
		if (NULL != map)
		{
			_tilePosition.x = rand() % map->GetWidth();
			_tilePosition.y = rand() % map->GetHeight();

			map->SetTileComponent(_tilePosition, this);
		}		
	}

	// font
	{
		_font = new Font(L"Arial", 15, D3DCOLOR_ARGB(255, 0, 0, 0));
		_font->SetRect(_position.x - 100, _position.y - 30, 200, 50);

		WCHAR text[100];
		wsprintf(text, L"HP %d", _hp);
		_font->SetText(text);
	}

	// StateMap  구성
	{
		State* state = new IdleState(this);
		state->CreateSprite(textureFilename, scriptFilename);
		_stateMap[eStateType::ST_IDLE] = state;
	}
	{
		State* state = new MoveState(this);
		state->CreateSprite(textureFilename, scriptFilename);
		_stateMap[eStateType::ST_MOVE] = state;
	}
	{
		State* state = new AttackState(this);
		state->CreateSprite(textureFilename, scriptFilename);
		_stateMap[eStateType::ST_ATTACK] = state;
	}
	{
		State* state = new DefenseState(this);
		state->CreateSprite(textureFilename, scriptFilename);
		_stateMap[eStateType::ST_DEFENSE] = state;
	}
	{
		State* state = new DeadState(this);
		state->CreateSprite(textureFilename, scriptFilename);
		_stateMap[eStateType::ST_DEAD] = state;
	}
	ChangeState(eStateType::ST_IDLE);
}

void Character::Deinit()
{
	for (std::map<eStateType, State*>::iterator it = _stateMap.begin();
		it != _stateMap.end();
		it++)
	{
		State* state = it->second;
		delete state;
	}
	_stateMap.clear();

	if (NULL != _font)
	{
		delete _font;
		_font = NULL;
	}
}

void Character::Update(float deltaTime)
{
	_state->Update(deltaTime);
	UpdateAttackCooltime(deltaTime);
	
	WCHAR text[100];
	int coolTime = (int)(_attackCooltimeDuration * 1000.0f);
	wsprintf(text, L"HP %d\n%d", _hp, coolTime);
	_font->SetText(text);
}

void Character::Render()
{
	_state->Render();
	_font->SetPosition(_position.x - 100, _position.y - 50);
	_font->Render();
}

void Character::Release()
{
	_font->Release();
	_state->Release();
}

void Character::Reset()
{
	_font->Reset();
	_state->Reset();
}

void Character::ReceiveMsg(const sMessageParam& param)
{
	if (L"Attack" == param.message)
	{
		_damagePoint = param.attackPoint;
		_state->ChangeState(eStateType::ST_DEFENSE);
	}
	if (L"RecoveryHP" == param.message)
	{
		//RecoverHP(param.recoveryHP);
		_hp += param.recoveryHP;
		if (100 < _hp)
			_hp = 100;
	}
}

void Character::ChangeState(eStateType stateType)
{
	if (NULL != _state)
		_state->Stop();

	_state = _stateMap[stateType];
	_state->Start();
}

void Character::UpdateAI(float deltaTime)
{
}

void Character::MoveStart(TilePoint newTilePosition)
{
	Map* map = (Map*)ComponentSystem::GetInstance()->FindComponent(L"Map");
	if (NULL != map)
	{
		map->ResetTileComponent(_tilePosition, this);
		_tilePosition = newTilePosition;
		map->SetTileComponent(_tilePosition, this);

		_isMoving = true;
	}
}

void Character::MoveStop()
{
	_isMoving = false;
}

std::vector<Component*> Character::Collision(std::vector<Component*> collisionList)
{
	return collisionList;
}

void Character::UpdateAttackCooltime(float deltaTime)
{
	if (_attackCooltimeDuration < _attackCooltime)
		_attackCooltimeDuration += deltaTime;
}

bool Character::IsAttackCooltime()
{
	if (_attackCooltime <= _attackCooltimeDuration)
		return true;
	return false;
}

void Character::ResetAttackCooltime()
{
	_attackCooltimeDuration = 0.0f;
}

void Character::DecreaseHP(int damagePoint)
{
	_hp -= damagePoint;
	if (_hp < 0)
	{
		_isLive = false;
		_hp = 0;
	}
}

void Character::EatItem()
{
	Map* map = (Map*)ComponentSystem::GetInstance()->FindComponent(L"Map");
	if (NULL != map)
	{
		Component* item = map->FindItemInTile(_tilePosition);
		if (NULL != item)
		{
			sMessageParam msg;
			msg.sender = this;
			msg.receiver = item;
			msg.message = L"Use";
			ComponentSystem::GetInstance()->SendMsg(msg);
		}
	}
}
