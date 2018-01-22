#pragma once

typedef struct _Point
{
	float x;
	float y;
} Point;

typedef struct _TilePoint
{
	int x;
	int y;
} TilePoint;

enum eDirection
{
	LEFT,
	RIGHT,
	UP,
	DOWN,
	NONE,
};
