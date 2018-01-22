#include "ComponentSystem.h"
#include "Character.h"
#include "AttackState.h"


AttackState::AttackState(Character* character) : State(character)
{
}

AttackState::~AttackState()
{
}

void AttackState::Start()
{
	State::Start();

	std::vector<Component*> targetList = _character->GetTargetList();
	for (int i = 0; i < targetList.size(); i++)
	{
		// 적이면 공격
		sMessageParam param;
		param.sender = _character;
		param.receiver = targetList[i];
		param.message = L"Attack";
		param.attackPoint = _character->GetAttackPoint();
		ComponentSystem::GetInstance()->SendMsg(param);
	}
}

void AttackState::Update(float deltaTime)
{
	State::Update(deltaTime);

	if (eStateType::ST_NONE != _nextState)
	{
		_character->ChangeState(_nextState);
		return;
	}

	_character->ResetTarget();
	_nextState = eStateType::ST_IDLE;
}