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

// 회전 각도 저장용.
float rot = 0.01f;
float pi = 3.141592f;
float doublePI = pi * 2.0f;
void Engine::Update()
{
	// 회전 각도 변경.
	rot += XMConvertToRadians(0.1f);
	if (rot >= doublePI)
		rot = 0.0f;

	// 회전 행렬.
	XMMATRIX rotation = XMMatrixRotationY(rot);
	// 이동 행렬.
	XMMATRIX translation = XMMatrixTranslation(3.0f, 1.0f, 0.0f);
	// 스케일 행렬.
	XMMATRIX scale = XMMatrixScaling(1.5f, 1.5f, 1.0f);
	
	// 월드 행렬 갱신.
	world = XMMatrixIdentity();
	//world = translation * rotation;
	world = scale * rotation * translation;

	// 셰이더 상수버퍼에 전달할 데이터 설정.
	CBPerObject cbData;
	cbData.world = XMMatrixTranspose(world);
	cbData.view = XMMatrixTranspose(view);
	cbData.projection = XMMatrixTranspose(projection);

	// 상수 버퍼 갱신.
	pDeviceContext->UpdateSubresource(cbBuffer, 0, 0, &cbData, 0, 0);

	// 파이프라인 바인딩.
	pDeviceContext->VSSetConstantBuffers(0, 1, &cbBuffer);
}

void Engine::Render()
{
	// 배경 지우기. (백버퍼 지우기).
	float color[4] = { 0.0f, 0.5f, 0.5f, 1.0f };
	pDeviceContext->ClearRenderTargetView(pRenderTargetView, color);

	// 정점 그리기.
	pDeviceContext->DrawIndexed(nIndices, 0, 0);

	// 버퍼 교체. (백버퍼 <-> 프론트버퍼).
	pSwapChain->Present(0, 0);
}