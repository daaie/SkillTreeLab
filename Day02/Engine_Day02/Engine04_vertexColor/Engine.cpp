#include "Engine.h"

using namespace Memory;

Engine::Engine(HINSTANCE hinstance) 
	: DXApp(hinstance) // 부모의 기본생성자
{
}

Engine::~Engine()
{
	
	//ID3D11Device *pDevice = NULL;
	//Memory::SafeRelease(pDevice);
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
	float color[4] = { 0.0f,0.4f,0.6f,1.0f }; // 마지막은 알파 값
	//배경 지우기
	pDeviceContext->ClearRenderTargetView(pRenderTargetView, color);

	//정점개수,정점읽기시작하는 위치
	pDeviceContext->Draw(3, 0);

	//버퍼 교체 ( 백버퍼 <-> 프론트버퍼 )
	pSwapChain->Present(0, 0);
	
}
