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

float rot = 0.01f;
float pi = 3.141592f;
float doublePI = pi * 2.0f;
void Engine::Update()
{
	rot += XMConvertToRadians(0.001f);
	if (rot >= doublePI)
		rot = 0.0f;

	XMMATRIX rotation = XMMatrixRotationY(rot);
	XMMATRIX scale = XMMatrixScaling(1.5f, 1.5f, 1.0f);

	world = XMMatrixIdentity();
	world = scale;

	CBPerObject cbData;
	cbData.world = XMMatrixTranspose(world);
	cbData.view = XMMatrixTranspose(view);
	cbData.projection = XMMatrixTranspose(projection);

	pDeviceContext->UpdateSubresource(cbBuffer, 0, 0, &cbData, 0, 0);
}

void Engine::Render()
{
	// ��� �����. (����� �����).
	float color[4] = { 0.0f, 0.5f, 0.5f, 1.0f };
	pDeviceContext->ClearRenderTargetView(pRenderTargetView, color);

	// ���� �׸���.
	pDeviceContext->DrawIndexed(nIndices, 0, 0);

	// ���� ��ü. (����� <-> ����Ʈ����).
	pSwapChain->Present(0, 0);
}