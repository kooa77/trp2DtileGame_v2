#pragma once

#include "Player.h"

class PathfinderPlayer : public Player
{
public:
	PathfinderPlayer(std::wstring name);
	~PathfinderPlayer();

public:
	void Init(std::wstring textureFilename, std::wstring scriptFilename);
	void UpdateAI(float deltaTime);

	void InitState(std::wstring textureFilename, std::wstring scriptFilename);
};
