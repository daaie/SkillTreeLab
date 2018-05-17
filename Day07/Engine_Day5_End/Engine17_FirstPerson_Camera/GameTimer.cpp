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
	// ���ļ� 
	LARGE_INTEGER frequencyCount;
	QueryPerformanceFrequency(&frequencyCount);
	countsPerSecond = double(frequencyCount.QuadPart);

	// ƽ ����
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
	

	//d���� ������ �ð� ����
	prevFrameTime = currentTime.QuadPart;

	if (tickCount < 0.0f)
	{
		tickCount = 0.0f;
	}// ��Ȥ ������ ���ü� �ִ�.

	double deltaTime = ((double)tickCount / countsPerSecond);

	if (deltaTime < 0.0f)
	{
		deltaTime = 0.0f;
	}// ��Ȥ ������ ���ü� �ִ�.

	return deltaTime;
}

void GameTimer::UpdateTime()
{
	// ƽ ������ ������ �� ������Ű��.
	++frameCount;

	// �ð� ����ؼ� 1�� ������ Ÿ�̸� �ʱ�ȭ.
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
