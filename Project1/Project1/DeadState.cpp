#include "Sprite.h"

#include "DeadState.h"


DeadState::DeadState(Character* character) : State(character)
{
}

DeadState::~DeadState()
{
}

void DeadState::Start()
{
	State::Start();
}

void DeadState::Update(float deltaTime)
{
	State::Update(deltaTime);
}

void DeadState::CreateSprite(std::wstring textureFilename, std::wstring scriptFilename)
{
	_spriteList.clear();
	{
		// left
		std::wstring script = scriptFilename;
		script = script.append(L"_left.json");
		Sprite* sprite = new Sprite();
		sprite->Init(textureFilename, script, 1.5f);
		_spriteList.push_back(sprite);
	}
	{
		// left
		std::wstring script = scriptFilename;
		script = script.append(L"_right.json");
		Sprite* sprite = new Sprite();
		sprite->Init(textureFilename, script, 1.5f);
		_spriteList.push_back(sprite);
	}
	{
		// left
		std::wstring script = scriptFilename;
		script = script.append(L"_up.json");
		Sprite* sprite = new Sprite();
		sprite->Init(textureFilename, script, 1.5f);
		_spriteList.push_back(sprite);
	}
	{
		// left
		std::wstring script = scriptFilename;
		script = script.append(L"_down.json", 1.5f);
		Sprite* sprite = new Sprite();
		sprite->Init(textureFilename, script);
		_spriteList.push_back(sprite);
	}
}