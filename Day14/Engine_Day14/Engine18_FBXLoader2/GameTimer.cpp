#include "GameTimer.h"



GameTimer::GameTimer()
{
}


GameTimer::~GameTimer()
{
}

void GameTimer::StartTime()
{
	LARGE_INTEGER frequencyCount;
	// WIN32 API.
	QueryPerformanceFrequency(&frequencyCount);

	countsPerSecond = (double)frequencyCount.QuadPart;

	// ƽ ������ �ð�.
	QueryPerformanceCounter(&frequencyCount);
	countStart = frequencyCount.QuadPart;
}

double GameTimer::GetTime()
{
	LARGE_INTEGER currentTime;
	QueryPerformanceCounter(&currentTime);

	double time 
		= (double)((currentTime.QuadPart - countStart) / countsPerSecond);

	return time;
}

double GameTimer::DeltaTime()
{
	// ƽ ������ ���� �ð� ���.
	LARGE_INTEGER currentTime;
	QueryPerformanceCounter(&currentTime);

	// ���� �����ӿ��� ���� �����ӱ��� ƽ ������ �󸶳� ���� �������� ���.
	__int64 tickCount = currentTime.QuadPart - prevFrameTime;
	
	// ���� ������ �ð� ����.
	prevFrameTime = currentTime.QuadPart;

	// ���� ó��.
	if (tickCount < 0.0f)
		tickCount = 0.0f;

	// ������ �ð��� �ʴ� �ð� ������ ��ȯ.
	double deltaTime = (double)(tickCount / countsPerSecond);
	
	// ���� ó��.
	if (deltaTime < 0.0)
		deltaTime = 0.0;

	return deltaTime;
}

void GameTimer::UpdateTimer()
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
