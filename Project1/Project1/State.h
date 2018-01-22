#pragma once

#include <string>
#include <vector>

class Character;
class Sprite;
enum eStateType;

class State
{
protected:
	Character* _character;
	eStateType _nextState;

	std::vector<Sprite*> _spriteList;

public:
	State(Character* character);
	~State() {}

public:
	virtual void Start();
	virtual void Stop() {}
	virtual void Update(float deltaTime);
	virtual void Render();

	virtual void Release();
	virtual void Reset();

	virtual void CreateSprite(std::wstring textureFilename, std::wstring scriptFilename);
	void ChangeState(eStateType nextState) { _nextState = nextState; }
};
