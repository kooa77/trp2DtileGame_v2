#pragma once

#include <d3dx9.h>
#include <string>

class Texture
{
private:
	//LPDIRECT3DDEVICE9 _device3d;
	IDirect3DTexture9* _textureDX;
	D3DXIMAGE_INFO _texInfo;

	std::wstring _fileName;
	
public:
	//Texture(LPDIRECT3DDEVICE9 device3d, std::wstring filePath);
	Texture(std::wstring filePath);
	~Texture();

	void Release();
	//void Reset(LPDIRECT3DDEVICE9 device3d);
	void Reset();

	IDirect3DTexture9* GetTextureDX();
};
