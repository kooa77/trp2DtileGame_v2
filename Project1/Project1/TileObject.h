#pragma once

#include "Component.h"

class Sprite;

class TileObject : public Component
{
private:
	Sprite* _sprite;

public:
	//TileObject(std::wstring name, Sprite* sprite);
	TileObject(std::wstring name, Sprite* sprite, int tileX, int tileY);
	~TileObject();

public:
	void Init(std::wstring textureFilename, std::wstring scriptFilename);
	void Deinit();
	void Update(float deltaTime);
	void Render();

	void Release();
	void Reset();
};
