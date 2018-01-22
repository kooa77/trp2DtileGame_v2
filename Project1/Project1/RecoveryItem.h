#pragma once

#include "Component.h"

class Sprite;

class RecoveryItem : public Component
{
private:
	Sprite* _sprite;

public:
	RecoveryItem(std::wstring name);
	~RecoveryItem();

public:
	void Init(std::wstring textureFilename, std::wstring scriptFilename);
	void Deinit();
	void Update(float deltaTime);
	void Render();

	void Release();
	void Reset();

	void ReceiveMsg(const sMessageParam& param);	// 메시지 처리를 위해 재정의
};
