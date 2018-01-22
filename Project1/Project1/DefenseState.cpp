#include "Character.h"

#include "DefenseState.h"


DefenseState::DefenseState(Character* character) : State(character)
{
}

DefenseState::~DefenseState()
{
}

void DefenseState::Start()
{
	State::Start();

	int damagePoint = _character->GetDamagePoint();
	_character->DecreaseHP(damagePoint);
	if( false == _character->IsLive() )
	{
		_character->SetCanMove(true);
		_character->MoveStop();
		ChangeState(eStateType::ST_DEAD);
	}
	else
	{
		ChangeState(eStateType::ST_IDLE);
	}
}

void DefenseState::Update(float deltaTime)
{
	State::Update(deltaTime);

	if (eStateType::ST_NONE != _nextState)
	{
		_character->ChangeState(_nextState);
		return;
	}
}

