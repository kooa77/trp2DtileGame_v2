#pragma once

#include <queue>
#include "State.h"

class Character;
class TileCell;

class PathfindingState : public State
{
public:
	PathfindingState(Character* character);
	~PathfindingState();

public:
	void Start();
	void Stop();
	void Update(float deltaTime);

	// Pathfinding
private:
	struct sPathCommand
	{
		TileCell* tileCell;
		float heuristic;	// 이 값이 작을수록 더 빨리 검사해야 할 타일이다
	};

	enum eUpdateState
	{
		PATHFINDING,
		BUILD_PATH
	};
	eUpdateState _updateState;

	struct compare
	{
		bool operator()(sPathCommand& a, sPathCommand& b)
		{
			return a.heuristic < b.heuristic;
		}
	};

	std::priority_queue<sPathCommand, std::vector<sPathCommand>, compare> _pathfindingQueue;
	TileCell* _targetTileCell;

	TileCell* _reverseTileCell;

	Map* _map;

	void UpdatePathfinding();
	void UpdateBuildPath();

	TilePoint GetSearchTilePositionByDirection(TilePoint tilePosition, eDirection direction);

	void CheckTestMark(TileCell* tileCell);
	void CheckBuildTestMark(TileCell* tileCell);
};
