#include "ComponentSystem.h"

ComponentSystem* ComponentSystem::_instance = 0;

ComponentSystem* ComponentSystem::GetInstance()
{
	if (0 == _instance)
	{
		_instance = new ComponentSystem();
	}
	return _instance;
}

void ComponentSystem::Update(float deltatime)
{
	ProcessMsgQueue();
}

void ComponentSystem::AddComponent(std::wstring name, Component* component)
{
	std::map<std::wstring, Component*>::iterator it = _componentMap.find(name);
	if (it == _componentMap.end())
	{
		_componentMap[name] = component;
	}
}

void ComponentSystem::RemoveAllComponents()
{
	for (std::map<std::wstring, Component*>::iterator it = _componentMap.begin();
		it != _componentMap.end(); it++)
	{
		(it->second)->Deinit();
		delete (it->second);
	}
	_componentMap.clear();
}

Component* ComponentSystem::FindComponent(std::wstring name)
{
	std::map<std::wstring, Component*>::iterator it = _componentMap.find(name);
	if (it != _componentMap.end())
	{
		return it->second;
	}
	return NULL;
}

void ComponentSystem::SendMsg(const sMessageParam& param)
{
	_msgQueue.push(param);
}

void ComponentSystem::ProcessMsgQueue()
{
	while (0 < _msgQueue.size())
	{
		sMessageParam param = _msgQueue.front();
		_msgQueue.pop();
		param.receiver->ReceiveMsg(param);
	}
}