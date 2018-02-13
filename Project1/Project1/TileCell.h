#pragma once

#include <list>
#include "GlobalTypes.h"

class Sprite;
class Component;

class TileCell
{
private:
	Point _position;

public:
	TileCell();
	~TileCell();

	void Init(int tileX, int tileY);
	void Update(float deltaTime);
	void Render();
	
	void Release();
	void Reset();

	void SetPosition(Point position);
	Point GetPosition() { return _position; }

	// Component List
private:
	std::list<Component*> _componentList;

public:
	void AddComponent(Component* component);
	void RemoveComponent(Component* component);

	std::list<Component*> GetCollisionList();
	std::list<Component*> GetComponentList() { return _componentList; }

	bool CanMove();

	// TilePosition
private:
	TilePoint _tilePostion;

public:
	TilePoint GetTilePosition() { return _tilePostion; }

	// Pathfinding
private:
	bool _pathfindingSearch;
	TileCell* _prevTileCell;
	float _distanceFromStart;

public:
	void ResetPathfinding()
	{
		_pathfindingSearch = false;
		_prevTileCell = NULL;
		_distanceFromStart = 0.0f;
	}

	bool IsSearchPathfinding() { return _pathfindingSearch; }
	void SearchPathfind() { _pathfindingSearch = true; }

	TileCell* GetPrevTileCell() { return _prevTileCell; }
	void SetPrevTileCell(TileCell* tileCell) { _prevTileCell = tileCell; }

	float GetDistanceFromStart() { return _distanceFromStart; }
	void SetDistanceFromStart(float distanceFromStart) { _distanceFromStart = distanceFromStart; }
};
