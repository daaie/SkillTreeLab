#include "Engine.h"



Engine::Engine(HINSTANCE hinstance) 
	: DXApp(hinstance) // 부모의 기본생성자
{
}

Engine::~Engine()
{
}

bool Engine::Init()
{
	if (DXApp::Init() == false)
		return false;

	return true;
}

void Engine::Update()
{
}

void Engine::Render()
{
}
