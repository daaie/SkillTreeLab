#pragma once
#include <Windows.h>

class GameTimer
{
public:
	GameTimer();
	~GameTimer();

	void StartTime();
	double GetTime();
	double DeltaTime();

	void UpdateTime();
	void Reset();
	int GetFPS();

protected:
	double countsPerSecond = 0.0;
	__int64 countStart = 0;

	int frameCount = 0;
	int fps = 0;

	__int64 prevFrameTime = 0;
};

