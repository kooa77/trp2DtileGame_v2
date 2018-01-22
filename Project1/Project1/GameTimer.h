#pragma once

class GameTimer
{
private:
	double _secondsPerCount;
	double _deltaTime;
	__int64 _prevCounter;
	
public:
	GameTimer();
	~GameTimer();

	void Init();
	void Update();
	double GetDeltaTime();
};
