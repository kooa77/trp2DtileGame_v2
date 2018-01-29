#include "Sprite.h"
#include "TileObject.h"


TileObject::TileObject(std::wstring name, Sprite* sprite, int tileX, int tileY) : Component(name)
{
	_sprite = sprite;
	_tilePosition.x = tileX;
	_tilePosition.y = tileY;
}

TileObject::~TileObject()
{
}

void TileObject::Init(std::wstring textureFilename, std::wstring scriptFilename)
{
}

void TileObject::Deinit()
{
}

void TileObject::Update(float deltaTime)
{
	_sprite->Update(deltaTime);
}

void TileObject::Render()
{
	_sprite->SetPosition(_position.x, _position.y);
	_sprite->Render();
}

void TileObject::Release()
{
	_sprite->Release();
}

void TileObject::Reset()
{
	_sprite->Reset();
}
