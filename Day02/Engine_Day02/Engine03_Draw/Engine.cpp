#include "Engine.h"

using namespace Memory;

Engine::Engine(HINSTANCE hinstance) 
	: DXApp(hinstance) // �θ��� �⺻������
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
	float color[4] = { 0.0f,0.4f,0.6f,1.0f }; // �������� ���� ��
	//��� �����
	pDeviceContext->ClearRenderTargetView(pRenderTargetView, color);

	//��������,�����б�����ϴ� ��ġ
	pDeviceContext->Draw(3, 0);

	//���� ��ü ( ����� <-> ����Ʈ���� )
	pSwapChain->Present(0, 0);
	
}
