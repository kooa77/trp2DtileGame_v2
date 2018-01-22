#include <fstream>

#include "Texture.h"
#include "ResourceManager.h"

ResourceManager* ResourceManager::_instance = 0;

ResourceManager* ResourceManager::GetInstance()
{
	if (0 == _instance)
	{
		_instance = new ResourceManager();
	}
	return _instance;
}

ResourceManager::ResourceManager()
{
}

ResourceManager::~ResourceManager()
{
}

//Texture* ResourceManager::FindTexture(std::wstring fileName, LPDIRECT3DDEVICE9 device3d)
Texture* ResourceManager::FindTexture(std::wstring fileName)
{
	WCHAR filePath[256];
	wsprintf(filePath, L"../Resources/Images/%s", fileName.c_str());

	std::map<std::wstring, Texture*>::iterator it = _textureMap.find(filePath);
	
	if (it != _textureMap.end())
	{
		return it->second;
	}
	
	Texture* texture = new Texture(filePath);
	_textureMap[filePath] = texture;

	return texture;
}

void ResourceManager::RemoveAllTexture()
{
	for (std::map<std::wstring, Texture*>::iterator it = _textureMap.begin();
		it != _textureMap.end(); it++)
	{
		Texture* tex = it->second;
		tex->Release();
		delete tex;
	}
	_textureMap.clear();
}

std::vector<std::string> ResourceManager::FindScript(std::wstring fileName)
{
	WCHAR filePath[256];
	wsprintf(filePath, L"../Resources/Scripts/%s", fileName.c_str());

	std::map<std::wstring, std::vector<std::string>>::iterator it = _scriptMap.find(filePath);
	if (it != _scriptMap.end())
	{
		return it->second;
	}

	std::vector<std::string> recordList;
	std::ifstream infile(filePath);
	if (infile.is_open())
	{
		while (!infile.eof())
		{
			char record[1000];
			infile.getline(record, 1000);
			recordList.push_back(record);
		}
		_scriptMap[filePath] = recordList;
	}

	return recordList;
}
