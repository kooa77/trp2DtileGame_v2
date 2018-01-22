#include "Sprite.h"
#include "Character.h"

#include "State.h"

State::State(Character* character)
{
	_character = character;
	_nextState = eStateType::ST_NONE;
}

void State::Start()
{
	_nextState = eStateType::ST_NONE;
}

void State::Update(float deltaTime)
{
	_spriteList[(int)_character->GetDirection()]->Update(deltaTime);
}

void State::Render()
{
	_spriteList[(int)_character->GetDirection()]->SetPosition(_character->GetPosition().x, _character->GetPosition().y);
	_spriteList[(int)_character->GetDirection()]->Render();
}

void State::Release()
{
	for (int i = 0; i < _spriteList.size(); i++)
	{
		_spriteList[i]->Release();
	}
}

void State::Reset()
{
	for (int i = 0; i < _spriteList.size(); i++)
	{
		_spriteList[i]->Reset();
	}
}

void State::CreateSprite(std::wstring textureFilename, std::wstring scriptFilename)
{
	_spriteList.clear();
	{
		// left
		std::wstring script = scriptFilename;
		script = script.append(L"_left.json");
		Sprite* sprite = new Sprite();
		sprite->Init(textureFilename, script);
		_spriteList.push_back(sprite);
	}
	{
		// left
		std::wstring script = scriptFilename;
		script = script.append(L"_right.json");
		Sprite* sprite = new Sprite();
		sprite->Init(textureFilename, script);
		_spriteList.push_back(sprite);
	}
	{
		// left
		std::wstring script = scriptFilename;
		script = script.append(L"_up.json");
		Sprite* sprite = new Sprite();
		sprite->Init(textureFilename, script);
		_spriteList.push_back(sprite);
	}
	{
		// left
		std::wstring script = scriptFilename;
		script = script.append(L"_down.json");
		Sprite* sprite = new Sprite();
		sprite->Init(textureFilename, script);
		_spriteList.push_back(sprite);
	}
}

