#include "ComponentSystem.h"
#include "Component.h"


Component::Component(std::wstring name)
{
	_type = eComponentType::CT_DEFAULT;

	_name = name;
	_tilePosition.x = _tilePosition.y = 0;
	_canMove = true;
	_isLive = true;
	ComponentSystem::GetInstance()->AddComponent(_name, this);
}

Component::~Component()
{

}

void Component::ReceiveMsg(const sMessageParam& param)
{
	if (L"Collision" == param.message)
	{
		// 충돌처리
	}
}