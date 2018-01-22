#pragma once

#include <string>
#include "ComponentMessage.h"
#include "GlobalTypes.h"

enum eComponentType
{
	CT_DEFAULT,
	CT_PLAYER,
	CT_NPC,
	CT_MONSTER,
	CT_ITEM,
};

class Component
{
protected:
	eComponentType _type;
	std::wstring _name;
	Point _position;
	bool _isLive;

public:
	Component(std::wstring name);
	virtual ~Component();

public:
	virtual void Init(std::wstring textureFilename, std::wstring scriptFilename) = 0;
	virtual void Deinit() = 0;
	virtual void Update(float deltaTime) = 0;
	virtual void Render() = 0;

	virtual void Release() = 0;
	virtual void Reset() = 0;

	void SetPosition(Point position) { _position = position; }
	Point GetPosition() { return _position; }

	eComponentType GetType() { return _type; }

	bool IsLive() { return _isLive;  }
	void SetLive(bool isLive) { _isLive = isLive; }

protected:
	TilePoint _tilePosition;

public:
	TilePoint GetTilePosition() { return _tilePosition; }

	// Message
public:
	virtual void ReceiveMsg(const sMessageParam& param);

	// Move
private:
	bool _canMove;

public:
	bool CanMove() { return _canMove; }
	void SetCanMove(bool canMove) { _canMove = canMove; }

};
