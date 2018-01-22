#pragma once

#include <d3dx9.h>

class GameSystem
{
	// Singleton
private:
	static GameSystem* _instance;

public:
	static GameSystem* GetInstance();

private:
	GameSystem() {}
	~GameSystem() {}

	// Windows
private:
	float _clientWidth;
	float _clientHeight;

public:
	float GetClientWidth() { return _clientWidth;  }
	void SetClientWidth(float width) { _clientWidth = width; }

	float GetClientHeight() { return _clientHeight; }
	void SetClientHeight(float height) { _clientHeight = height; }

	// Graphics
private:
	LPDIRECT3DDEVICE9 _device3d;
	ID3DXSprite* _spriteDX;

public:
	LPDIRECT3DDEVICE9 GetDeviceDX() { return _device3d; }
	void SetDeviceDX(LPDIRECT3DDEVICE9 device3d) { _device3d = device3d; }

	ID3DXSprite* GetSpriteDX() { return _spriteDX; }
	void SetSpriteDX(ID3DXSprite* spriteDX) { _spriteDX = spriteDX; }
};
