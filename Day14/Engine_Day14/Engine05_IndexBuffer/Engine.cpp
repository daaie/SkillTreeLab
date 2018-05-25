#include "Engine.h"
#include <d3d11.h>

Engine::Engine(HINSTANCE hinstance) 
	: DXApp(hinstance)
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
	// 배경 지우기. (백버퍼 지우기).
	float color[4] = { 0.0f, 0.5f, 0.5f, 1.0f };
	pDeviceContext->ClearRenderTargetView(pRenderTargetView, color);

	// 정점 그리기.
	//pDeviceContext->Draw(3, 0);
	pDeviceContext->DrawIndexed(6, 0, 0);

	// 버퍼 교체. (백버퍼 <-> 프론트버퍼).
	pSwapChain->Present(0, 0);
}
