#pragma once

#include <d3dx9.h>
#include <map>
#include <string>
#include <vector>
#include <stack>
#include "GlobalTypes.h"
#include "Component.h"

enum eStateType
{
	ST_NONE,
	ST_IDLE,
	ST_MOVE,
	ST_ATTACK,
	ST_DEFENSE,
	ST_DEAD,
	ST_PATHFINDING,
};

class Sprite;
class Font;
class State;
class TileCell;

class Character : public Component
{
protected:
	eDirection _currentDirection;

public:
	Character(std::wstring name);
	~Character();

	void Init(std::wstring textureFilename, std::wstring scriptFilename);
	void Deinit();
	void Update(float deltaTime);
	void Render();
	
	void Release();
	void Reset();

	eDirection GetDirection() { return _currentDirection; }
	
	// Common Info
protected:
	int _hp;
	int _attackPoint;

public:
	int GetAttackPoint() { return _attackPoint; }

	// Message
public:
	void ReceiveMsg(const sMessageParam& param);

	// State
protected:
	std::map<eStateType, State*> _stateMap;
	State* _state;

public:
	virtual void InitState(std::wstring textureFilename, std::wstring scriptFilename);
	void ChangeState(eStateType stateType);

	// AI
protected:
	float _moveTime;
	bool _isMoving;
	eDirection _nextDirection;
	std::vector<Component*> _targetList;

	std::stack<TileCell*> _pathfindingCellStack;

public:
	virtual void UpdateAI(float deltaTime);
	void MoveStart(TilePoint newTilePosition);
	void MoveStop();
	bool IsMoving() { return _isMoving; }
	float GetMoveTime() { return _moveTime; }

	void SetDirection(eDirection direction) { _currentDirection = direction; }
	eDirection GetNextDirection() { return _nextDirection; }

	virtual std::vector<Component*> Collision(std::vector<Component*> collisionList);

	void SetTarget(std::vector<Component*> targetList) { _targetList = targetList; }
	std::vector<Component*> GetTargetList() { return _targetList; }
	void ResetTarget() { _targetList.clear(); }

	void PushPathfindingCell(TileCell* tileCell) { _pathfindingCellStack.push(tileCell); }
	TileCell* PopPathfindingCell();
	bool IsEmptyPathfindingStack();

	// CoolTime
protected:
	float _attackCooltime;
	float _attackCooltimeDuration;

public:
	void UpdateAttackCooltime(float deltaTime);
	bool IsAttackCooltime();
	void ResetAttackCooltime();

	// Damage
private:
	int _damagePoint;

public:
	int GetDamagePoint() { return _damagePoint; }
	void DecreaseHP(int damagePoint);

	// Item
public:
	void EatItem();

	// UI
private:
	Font* _font;

	// Pathfinding
protected:
	TileCell* _targetTileCell;

public:
	void SetTargetTileCell(TileCell* targetTileCell)
	{
		_targetTileCell = targetTileCell;
	}
	TileCell* GetTargetTileCell() { return _targetTileCell; }
};
