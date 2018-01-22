#include "InputSystem.h"


InputSystem* InputSystem::_instance = 0;

InputSystem* InputSystem::GetInstance()
{
	if (0 == _instance)
	{
		_instance = new InputSystem();
	}
	return _instance;
}

InputSystem::InputSystem()
{
	for (int i = 0; i < 256; i++)
	{
		_keyState[i] = eKeyState::KEY_UP;
	}
}

void InputSystem::KeyDown(unsigned int keyCode)
{
	_keyState[keyCode] = eKeyState::KEY_DOWN;
}

void InputSystem::KeyUp(unsigned int keyCode)
{
	_keyState[keyCode] = eKeyState::KEY_UP;
}

bool InputSystem::IsKeyDown(unsigned int keyCode)
{
	if (eKeyState::KEY_DOWN == _keyState[keyCode])
		return true;
	return false;
}