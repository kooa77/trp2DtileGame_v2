#include "GameSystem.h"

#include "Texture.h"


//Texture::Texture(LPDIRECT3DDEVICE9 device3d, std::wstring filePath)
Texture::Texture(std::wstring filePath)
{
	//_device3d = device3d;
	_fileName = filePath;

	// 이미지 파일로 부터 이미지 정보를 얻어온다.
	HRESULT hr = D3DXGetImageInfoFromFile(filePath.c_str(), &_texInfo);
	if (FAILED(hr))
	{
		return;
	}

	//hr = D3DXCreateTextureFromFileEx(_device3d,
	hr = D3DXCreateTextureFromFileEx(GameSystem::GetInstance()->GetDeviceDX(),
		filePath.c_str(),
		_texInfo.Width, _texInfo.Height,
		1, 0,
		D3DFMT_UNKNOWN,
		D3DPOOL_DEFAULT,
		D3DX_DEFAULT, D3DX_DEFAULT,
		D3DCOLOR_ARGB(255, 255, 255, 255),
		&_texInfo,
		NULL,
		&_textureDX);
	if (FAILED(hr))
	{
		return;
	}
}

Texture::~Texture()
{
	Release();
}

void Texture::Release()
{
	if (_textureDX)
	{
		_textureDX->Release();
		_textureDX = NULL;
	}
}

//void Texture::Reset(LPDIRECT3DDEVICE9 device3d)
void Texture::Reset()
{
	//_device3d = device3d;
	
	// Texture 다시 로드
	//HRESULT hr = D3DXCreateTextureFromFileEx(_device3d,
	HRESULT hr = D3DXCreateTextureFromFileEx(GameSystem::GetInstance()->GetDeviceDX(),
		_fileName.c_str(),
		_texInfo.Width, _texInfo.Height,
		1, 0,
		D3DFMT_UNKNOWN,
		D3DPOOL_DEFAULT,
		D3DX_DEFAULT, D3DX_DEFAULT,
		D3DCOLOR_ARGB(255, 255, 255, 255),
		&_texInfo,
		NULL,
		&_textureDX);
}

IDirect3DTexture9* Texture::GetTextureDX()
{
	return _textureDX;
}
