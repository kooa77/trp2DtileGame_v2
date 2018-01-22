#pragma once

class InputSystem
{
	// Singleton
private:
	static InputSystem* _instance;

public:
	static InputSystem* GetInstance();

private:
	InputSystem();
	~InputSystem() {}

public:
	enum eKeyState
	{
		KEY_DOWN,
		KEY_UP
	};

private:
	unsigned int _keyState[256];

public:
	void KeyDown(unsigned int keyCode);
	void KeyUp(unsigned int keyCode);
	bool IsKeyDown(unsigned int keyCode);
};
