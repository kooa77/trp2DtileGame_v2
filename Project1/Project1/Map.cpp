#include <string>

#include "GameSystem.h"
#include "ResourceManager.h"
#include "Sprite.h"
#include "TileCell.h"
#include "TileObject.h"
#include "Map.h"


Map::Map(std::wstring name) : Component(name)
{
}

Map::~Map()
{
}

void Map::Init(std::wstring textureFilename, std::wstring scriptFilename)
{
	_width = mapWidth;
	_height = mapHeight;

	// Sprite List 구성
	{
		int srcX = 0;
		int srcY = 0;
		int spriteSize = 32;
		for (int y = 0; y < 16; y++)
		{
			for (int x = 0; x < 16; x++)
			{
				Sprite* sprite = new Sprite();
				sprite->Init(textureFilename, srcX, srcY, spriteSize, spriteSize, 1.0f);
				_spriteList.push_back(sprite);

				srcX += spriteSize;
			}
			srcX = 0;
			srcY += spriteSize;
		}
	}

	// 타일맵 인덱스 구성 -> 스크립트를 바탕으로 스프라이트를 생성
	_tileArray.clear();

	//CreateTiles(scriptFilename);
	CreateMazeTiles(scriptFilename);

	_tileSize = 32;
	_renderWidth = GameSystem::GetInstance()->GetClientWidth() / _tileSize + 1;
	_renderHeight = GameSystem::GetInstance()->GetClientHeight() / _tileSize + 1;

	_startX = _startY = 0;
}

void Map::Deinit()
{
	// 버그 수정
	for (int i = 0; i < _spriteList.size(); i++)
	{
		delete _spriteList[i];
	}
	_spriteList.clear();
}

void Map::Update(float deltaTime)
{
	for (int y = 0; y < _height; y++)
	{
		for (int x = 0; x < _width; x++)
		{
			_tileArray[y][x]->Update(deltaTime);
		}
	}

	UpdateViewer(deltaTime);
}

void Map::Render()
{
	int midTileCountX = _renderWidth / 2;
	int midTileCountY = _renderHeight / 2;

	int startTileX = _viewer->GetTilePosition().x - midTileCountX - 1;
	int startTileY = _viewer->GetTilePosition().y - midTileCountY - 1;
	int endTileX = startTileX + _renderWidth + 1;
	int endTileY = startTileY + _renderHeight + 1;

	if (_width < endTileX)
		endTileX = _width;
	if (_height < endTileY)
		endTileY = _height;

	Point renderPosition;
	renderPosition.x = renderPosition.y = 0.0f;
	for (int y = startTileY; y < endTileY; y++)
	{
		if (0 <= y)
		{
			for (int x = startTileX; x < endTileX; x++)
			{
				if (0 <= x)
				{
					_tileArray[y][x]->SetPosition(renderPosition);
					_tileArray[y][x]->Render();
				}
				renderPosition.x += _tileSize;
			}
		}
		renderPosition.x = 0.0f;
		renderPosition.y += _tileSize;
	}
}

void Map::Release()
{
	for (int y = 0; y < _height; y++)
	{
		for (int x = 0; x < _width; x++)
		{
			_tileArray[y][x]->Release();
		}
	}
}

void Map::Reset()
{
	for (int y = 0; y < _height; y++)
	{
		for (int x = 0; x < _width; x++)
		{
			_tileArray[y][x]->Reset();
		}
	}
}

void Map::CreateTiles(std::wstring scriptFilename)
{
	// 1 층
	{
		std::wstring layerScriptName = scriptFilename;
		layerScriptName = layerScriptName.append(L"01.csv");
		std::vector<std::string> recordList = ResourceManager::GetInstance()->FindScript(layerScriptName);

		char record[1000];
		char* token;

		// 첫 째 라인은 맵 크기 정보
		strcpy_s(record, recordList[0].c_str());
		token = strtok(record, ",");		// 첫번 째 칸은 스킵 : mapsize
		token = strtok(NULL, ",");			// 두번 째 실제 가로 크기 : 16
		_width = atoi(token);
		token = strtok(NULL, ",");			// 세번 째 실제 세로 크기 : 10
		_height = atoi(token);

		// 둘 째 라인은 스킵

		// 세번 째 라인 부터가 실제 맵 인덱스 데이타
		int line = 2;
		for (int y = 0; y < _height; y++)
		{
			strcpy_s(record, recordList[line].c_str());
			token = strtok(record, ",");

			std::vector<TileCell*> rowList;
			for (int x = 0; x < _width; x++)
			{
				int spriteIndex = atoi(token);
				Sprite* sprite = _spriteList[spriteIndex];

				TileCell* tileCell = new TileCell();
				tileCell->Init(x, y);

				// 타일 오브젝트를 생성
				WCHAR name[256];
				wsprintf(name, L"map_layer01_%d_%d", line, x);
				TileObject* tileObject = new TileObject(name, sprite, x, y);
				tileCell->AddComponent(tileObject);

				rowList.push_back(tileCell);

				token = strtok(NULL, ",");
			}
			_tileArray.push_back(rowList);
			line++;
		}
	}

	// 2 층
	{
		std::wstring layerScriptName = scriptFilename;
		layerScriptName = layerScriptName.append(L"02.csv");
		std::vector<std::string> recordList = ResourceManager::GetInstance()->FindScript(layerScriptName);

		char record[1000];
		char* token;

		// 첫 째 라인은 맵 크기 정보
		strcpy_s(record, recordList[0].c_str());
		token = strtok(record, ",");		// 첫번 째 칸은 스킵 : mapsize
		token = strtok(NULL, ",");			// 두번 째 실제 가로 크기 : 16
		_width = atoi(token);
		token = strtok(NULL, ",");			// 세번 째 실제 세로 크기 : 10
		_height = atoi(token);

		// 둘 째 라인은 스킵

		// 세번 째 라인 부터가 실제 맵 인덱스 데이타
		int line = 2;
		for (int y = 0; y < _height; y++)
		{
			strcpy_s(record, recordList[line].c_str());
			token = strtok(record, ",");

			std::vector<TileCell*> rowList = _tileArray[y];
			for (int x = 0; x < _width; x++)
			{
				int spriteIndex = atoi(token);
				if (0 <= spriteIndex)
				{
					Sprite* sprite = _spriteList[spriteIndex];

					TileCell* tileCell = rowList[x];

					// 타일 오브젝트를 생성
					WCHAR name[256];
					wsprintf(name, L"map_layer02_%d_%d", line, x);
					TileObject* tileObject = new TileObject(name, sprite, x, y);
					tileObject->SetCanMove(false);
					tileCell->AddComponent(tileObject);
				}

				token = strtok(NULL, ",");
			}
			_tileArray.push_back(rowList);
			line++;
		}
	}
}

void Map::CreateMazeTiles(std::wstring scriptFilename)
{
	// 1 층
	{
		std::wstring layerScriptName = scriptFilename;
		layerScriptName = layerScriptName.append(L"01.csv");
		std::vector<std::string> recordList = ResourceManager::GetInstance()->FindScript(layerScriptName);

		char record[1000];
		char* token;

		// 첫 째 라인은 맵 크기 정보
		strcpy_s(record, recordList[0].c_str());
		token = strtok(record, ",");		// 첫번 째 칸은 스킵 : mapsize
		token = strtok(NULL, ",");			// 두번 째 실제 가로 크기 : 16
		_width = atoi(token);
		token = strtok(NULL, ",");			// 세번 째 실제 세로 크기 : 10
		_height = atoi(token);

		// 둘 째 라인은 스킵

		// 세번 째 라인 부터가 실제 맵 인덱스 데이타
		int line = 2;
		for (int y = 0; y < _height; y++)
		{
			strcpy_s(record, recordList[line].c_str());
			token = strtok(record, ",");

			std::vector<TileCell*> rowList;
			for (int x = 0; x < _width; x++)
			{
				int spriteIndex = atoi(token);
				Sprite* sprite = _spriteList[spriteIndex];

				TileCell* tileCell = new TileCell();
				tileCell->Init(x, y);

				// 타일 오브젝트를 생성
				WCHAR name[256];
				wsprintf(name, L"map_layer01_%d_%d", line, x);
				TileObject* tileObject = new TileObject(name, sprite, x, y);
				tileCell->AddComponent(tileObject);

				rowList.push_back(tileCell);

				token = strtok(NULL, ",");
			}
			_tileArray.push_back(rowList);
			line++;
		}
	}

	// 기준이 되는 블럭을 심는다.
	for (int y = 0; y < _height; y++)
	{
		if (0 == (y % 2))
		{
			std::vector<TileCell*> rowList = _tileArray[y];
			for (int x = 0; x < _width; x++)
			{
				if (0 == (x % 2))
				{
					int spriteIndex = 139;
					if (0 <= spriteIndex)
					{
						Sprite* sprite = _spriteList[spriteIndex];

						TileCell* tileCell = rowList[x];

						// 타일 오브젝트를 생성
						WCHAR name[256];
						wsprintf(name, L"map_layer02_%d_%d", y, x);
						TileObject* tileObject = new TileObject(name, sprite, x, y);
						tileObject->SetCanMove(false);
						tileCell->AddComponent(tileObject);
					}
				}
			}
		}
	}

	// 기준이 되는 블럭을 연결해 나간다.
	for (int y = 0; y < _height; y++)
	{
		//std::vector<TileCell*> rowList = _tileArray[y];
		for (int x = 0; x < _width; x++)
		{
			TilePoint pos;
			pos.x = x;
			pos.y = y;
			if (false == GetTileCell(pos)->CanMove())
			{
				if (false == IsConnectedCell(x, y))
				{
					// 한 방향을 결정해서, 다음 블럭을 만날 때 까지(연결 될 때 까지) 이어준다.
					eDirection direction = (eDirection)(rand() % (int)eDirection::NONE);

					// 방향에 따른 다음 타일 위치를 구해서, 그 위치에 블럭 생성
					int newTileX = x;
					int newTileY = y;
					while (false == IsConnectedCell(newTileX, newTileY))
					{
						switch (direction)
						{
						case eDirection::LEFT: newTileX--; break;
						case eDirection::RIGHT: newTileX++; break;
						case eDirection::UP: newTileY--; break;
						case eDirection::DOWN: newTileY++; break;
						}
						if (0 <= newTileX && newTileX < _width &&
							0 <= newTileY && newTileY < _height)
						{
							int spriteIndex = 139;
							if (0 <= spriteIndex)
							{
								Sprite* sprite = _spriteList[spriteIndex];

								std::vector<TileCell*> rowList = _tileArray[newTileY];
								TileCell* tileCell = rowList[newTileX];

								// 타일 오브젝트를 생성
								WCHAR name[256];
								wsprintf(name, L"map_layer02_%d_%d", newTileY, newTileX);
								TileObject* tileObject = new TileObject(name, sprite, newTileX, newTileY);
								tileObject->SetCanMove(false);
								tileCell->AddComponent(tileObject);
							}
						}
					}					
				}
			}
		}
	}
}

bool Map::IsConnectedCell(int tileX, int tileY)
{
	// 주변에 하나라도 붙은 블럭이 있으면 연결된 블럭이다.
	for (int direction = 0; direction < (int)eDirection::NONE; direction++)
	{
		int searchTileX = tileX;
		int searchTileY = tileY;
		switch (direction)
		{
		case eDirection::LEFT: searchTileX--; break;
		case eDirection::RIGHT: searchTileX++; break;
		case eDirection::UP: searchTileY--; break;
		case eDirection::DOWN: searchTileY++; break;
		}
		if (0 <= searchTileX && searchTileX < _width &&
			0 <= searchTileY && searchTileY < _height)
		{
			TilePoint position;
			position.x = searchTileX;
			position.y = searchTileY;
			if (false == GetTileCell(position)->CanMove())
				return true;
		}
	}
	return false;
}

void Map::SetViewer(Component* component)
{
	_viewer = component;
	_prevViewTilePositon = _viewer->GetTilePosition();
}

void Map::UpdateViewer(float deltaTime)
{
	if (NULL == _viewer)
		return;

	if (_prevViewTilePositon.x != _viewer->GetTilePosition().x ||
		_prevViewTilePositon.y != _viewer->GetTilePosition().y)
	{
		if (_prevViewTilePositon.x < _viewer->GetTilePosition().x)
			MoveRight();
		if ( _viewer->GetTilePosition().x < _prevViewTilePositon.x)
			MoveLeft();
		if (_prevViewTilePositon.y < _viewer->GetTilePosition().y)
			MoveDown();
		if (_viewer->GetTilePosition().y < _prevViewTilePositon.y)
			MoveUp();

		_prevViewTilePositon = _viewer->GetTilePosition();
	}
}

void Map::SetTileComponent(TilePoint tilePosition, Component* component)
{
	_tileArray[tilePosition.y][tilePosition.x]->AddComponent(component);
}

void Map::ResetTileComponent(TilePoint tilePosition, Component* component)
{
	_tileArray[tilePosition.y][tilePosition.x]->RemoveComponent(component);
}

Point Map::GetPosition(int tileX, int tileY)
{
	Point point;
	point.x = tileX * _tileSize;
	point.y = tileY * _tileSize;
	return point;
}

std::vector<Component*> Map::GetTileCollisionList(TilePoint tilePosition)
{
	std::vector<Component*> collisiontArray;
	
	// 범위 체크 (맵 안에 있는지)
	if (tilePosition.x < 0 || GetWidth() <= tilePosition.x ||
		tilePosition.y < 0 || GetHeight() <= tilePosition.y)
		return collisiontArray;

	std::list<Component*> tileCollisionList = GetTileCell(tilePosition)->GetCollisionList();
	for (std::list<Component*>::iterator it = tileCollisionList.begin();
		it != tileCollisionList.end(); it++)
	{
		collisiontArray.push_back((*it));
	}
	return collisiontArray;
}

Component* Map::FindComponentInRange(Component* finder, int range, std::vector<eComponentType> findTypeList)
{
	int rangeMinX = finder->GetTilePosition().x - range;
	int rangeMaxX = finder->GetTilePosition().x + range;
	int rangeMinY = finder->GetTilePosition().y - range;
	int rangeMaxY = finder->GetTilePosition().y + range;

	if (rangeMinX < 0) rangeMinX = 0;
	if (GetWidth() <= rangeMaxX) rangeMaxX = GetWidth() - 1;
	if (rangeMinY < 0) rangeMinY = 0;
	if (GetHeight() <= rangeMaxY) rangeMaxY = GetHeight() - 1;

	Component* findComponent = NULL;
	for (int y = rangeMinY; y < rangeMaxY; y++)
	{
		for (int x = rangeMinX; x < rangeMaxX; x++)
		{
			TilePoint tilePosition;
			tilePosition.x = x;
			tilePosition.y = y;
			TileCell* tileCell = GetTileCell(tilePosition);
			std::list<Component*> collsionList = tileCell->GetCollisionList();
			if (0 < collsionList.size())
			{
				for (std::list<Component*>::iterator it = collsionList.begin();
					it != collsionList.end(); it++)
				{
					Component* component = (*it);
					if (component->IsLive())
					{
						for (int i = 0; i < findTypeList.size(); i++)
						{
							if (findTypeList[i] == component->GetType())
								return component;
						}
					}
				}
			}
		}
	}

	return NULL;
}

bool Map::CanMoveTile(TilePoint tilePosition)
{
	if (0 <= tilePosition.x && tilePosition.x < GetWidth() &&
		0 <= tilePosition.y && tilePosition.y < GetHeight())
	{
		return GetTileCell(tilePosition)->CanMove();
	}
	return false;
}

TileCell* Map::GetTileCell(TilePoint tilePosition)
{
	if (tilePosition.x < 0 || _width <= tilePosition.x ||
		tilePosition.y < 0 || _height <= tilePosition.y)
		return NULL;
	return _tileArray[tilePosition.y][tilePosition.x];
}

TileCell* Map::FindTileCellByMousePosition(int mouseX, int mouseY)
{
	int midX = GameSystem::GetInstance()->GetClientWidth() / 2;
	int midY = GameSystem::GetInstance()->GetClientHeight() / 2;

	int minX = _viewer->GetTilePosition().x - (midX / _tileSize) - 2;
	int maxX = _viewer->GetTilePosition().x + (midX / _tileSize) + 2;
	int minY = _viewer->GetTilePosition().y - (midY / _tileSize) - 2;
	int maxY = _viewer->GetTilePosition().y + (midY / _tileSize) + 2;

	if (minX < 0) minX = 0;
	if (_width < maxX) maxX = _width;
	if (minY < 0) minY = 0;
	if (_height < maxY) maxY = _height;

	for (int y = minY; y < maxY; y++)
	{
		for (int x = minX; x < maxX; x++)
		{
			TilePoint tilePosition;
			tilePosition.x = x;
			tilePosition.y = y;

			RECT rect;
			rect.left = GetTileCell(tilePosition)->GetPosition().x - _tileSize / 2;
			rect.right = GetTileCell(tilePosition)->GetPosition().x + _tileSize / 2;
			rect.top = GetTileCell(tilePosition)->GetPosition().y - _tileSize / 2;
			rect.bottom = GetTileCell(tilePosition)->GetPosition().y + _tileSize / 2;

			if (rect.left <= mouseX && mouseX <= rect.right &&
				rect.top <= mouseY && mouseY <= rect.bottom)
				return GetTileCell(tilePosition);
		}
	}
	return NULL;
}

std::vector<Component*> Map::GetTileComponentList(TilePoint tilePosition)
{
	std::vector<Component*> findArray;

	// 범위 체크 (맵 안에 있는지)
	if (tilePosition.x < 0 || GetWidth() <= tilePosition.x ||
		tilePosition.y < 0 || GetHeight() <= tilePosition.y)
		return findArray;

	std::list<Component*> tileCollisionList = GetTileCell(tilePosition)->GetComponentList();
	for (std::list<Component*>::iterator it = tileCollisionList.begin();
		it != tileCollisionList.end(); it++)
	{
		findArray.push_back((*it));
	}
	return findArray;
}

Component* Map::FindItemInTile(TilePoint tilePosition)
{
	std::vector<Component*> componentList = GetTileComponentList(tilePosition);
	for (int i = 0; i < componentList.size(); i++)
	{
		Component* component = componentList[i];
		if (eComponentType::CT_ITEM == component->GetType())
			return component;
	}
	return NULL;
}

void Map::MoveLeft()
{
	_startX--;
}

void Map::MoveRight()
{
	_startX++;
}

void Map::MoveUp()
{
	_startY--;
}

void Map::MoveDown()
{
	_startY++;
}

void Map::ResetPahtfinding()
{
	for (int y = 0; y < _height; y++)
	{
		for (int x = 0; x < _width; x++)
		{
			TilePoint tilePosition;
			tilePosition.x = x;
			tilePosition.y = y;
			GetTileCell(tilePosition)->ResetPathfinding();
		}
	}
}