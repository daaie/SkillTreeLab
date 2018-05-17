#include "GameTimer.h"



GameTimer::GameTimer()
{
}


GameTimer::~GameTimer()
{
}

void GameTimer::StartTime()
{
	// Win32 API
	// 주파수 
	LARGE_INTEGER frequencyCount;
	QueryPerformanceFrequency(&frequencyCount);
	countsPerSecond = double(frequencyCount.QuadPart);

	// 틱 개수
	QueryPerformanceCounter(&frequencyCount);
	countStart = frequencyCount.QuadPart;
}

double GameTimer::GetTime()
{
	LARGE_INTEGER currentTime;
	QueryPerformanceCounter(&currentTime);

	double time
		= (currentTime.QuadPart - countStart) / countsPerSecond;
	
	return time;
}

double GameTimer::DeltaTime()
{
	LARGE_INTEGER currentTime;
	QueryPerformanceCounter(&currentTime);

	__int64 tickCount = currentTime.QuadPart - prevFrameTime;
	

	//d이전 프레임 시간 저장
	prevFrameTime = currentTime.QuadPart;

	if (tickCount < 0.0f)
	{
		tickCount = 0.0f;
	}// 간혹 음수로 나올수 있다.

	double deltaTime = ((double)tickCount / countsPerSecond);

	if (deltaTime < 0.0f)
	{
		deltaTime = 0.0f;
	}// 간혹 음수로 나올수 있다.

	return deltaTime;
}

void GameTimer::UpdateTime()
{
	// 틱 단위로 프레임 수 증가시키기.
	++frameCount;

	// 시간 계산해서 1초 지나면 타이머 초기화.
	if (GetTime() > 1.0f)
	{
		Reset();
	}
}

void GameTimer::Reset()
{
	fps = frameCount;
	frameCount = 0;
	StartTime();
}

int GameTimer::GetFPS()
{
	return fps;
}
