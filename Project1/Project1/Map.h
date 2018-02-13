#pragma once

#include <d3dx9.h>
#include <vector>
#include <string>
#include "GlobalTypes.h"
#include "Component.h"

#define mapWidth 16
#define mapHeight 16 

class Sprite;
class TileCell;

class Map : public Component
{
private:
	std::vector<Sprite*> _spriteList;

	std::vector<std::vector<TileCell*>> _tileArray;
	
	int _width;
	int _height;

	int _tileSize;
	int _renderWidth;
	int _renderHeight;

public:
	Map(std::wstring name);
	~Map();

	void Init(std::wstring textureFilename, std::wstring scriptFilename);
	void Deinit();
	void Update(float deltaTime);
	void Render();

	void Release();
	void Reset();

	// Map Info
	int GetWidth() { return _width; }
	int GetHeight() { return _height; }

	void CreateTiles(std::wstring scriptFilename);
	void CreateMazeTiles(std::wstring scriptFilename);
	bool IsConnectedCell(int tileX, int tileY);

	// Viewer
private:
	Component* _viewer;
	TilePoint _prevViewTilePositon;

public:
	void SetViewer(Component* component);
	void UpdateViewer(float deltaTime);

	// Move
private:
	int _startX;
	int _startY;

public:
	void SetTileComponent(TilePoint tilePosition, Component* component);
	void ResetTileComponent(TilePoint tilePosition, Component* component);
	Point GetPosition(int tileX, int tileY);

	std::vector<Component*> GetTileCollisionList(TilePoint tilePosition);
	Component* FindComponentInRange(Component* finderr, int range, std::vector<eComponentType> findTypeList);
	bool CanMoveTile(TilePoint tilePosition);
	TileCell* GetTileCell(TilePoint tilePosition);
	TileCell* FindTileCellByMousePosition(int x, int y);

	std::vector<Component*> GetTileComponentList(TilePoint tilePosition);

	Component* FindItemInTile(TilePoint tilePosition);

	void MoveLeft();
	void MoveRight();
	void MoveUp();
	void MoveDown();

	// Pathfinding
public:
	void ResetPahtfinding();
};
