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

void Engine::Update(double deltaTime)
{
	// ī�޶� ����.
	updateCamera();

	//��ü ȸ����Ű��.
	RotateObejct(deltaTime);
}


// ȸ�� ���� �����.
float rot = 0.01f;
float pi = 3.141592f;
float doublePI = pi * 2.0f;
void Engine::RotateObejct(double deltaTime)
{
	// ������� �ʱ�ȭ
	world = XMMatrixIdentity();
	
	float rotBase = XMConvertToRadians(-90.0f);
	XMMATRIX rotation = XMMatrixRotationX(rotBase);

	// x/z�� ȸ�� ���.
	rotationX = XMMatrixRotationX(rotX);
	rotationZ = XMMatrixRotationY(rotZ);

	// ������Ŀ� ����.
	world = rotation* rotationX*rotationZ;

	// ���̴� ������ۿ� ������ ������ ����.
	CBPerObject cbData;
	cbData.world = XMMatrixTranspose(world);
	cbData.view = XMMatrixTranspose(view);
	cbData.projection = XMMatrixTranspose(projection);

	//// ��� ���� ����.
	pDeviceContext->UpdateSubresource(cbBuffer, 0, 0, &cbData, 0, 0);

	// ���������� ���ε�.
	//pDeviceContext->VSSetConstantBuffers(0, 1, &cbBuffer);
}

void Engine::Render()
{
	// ��� �����. (����� �����).
	float color[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
	pDeviceContext->ClearRenderTargetView(pRenderTargetView, color);

	// ����/ ���ٽ� �����
	pDeviceContext->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	// ���� �׸���.
	//pDeviceContext->DrawIndexed(nIndices, 0, 0);
	pDeviceContext->Draw(nVertices,0);

	// ���� ��ü. (����� <-> ����Ʈ����).
	pSwapChain->Present(0, 0);
}
