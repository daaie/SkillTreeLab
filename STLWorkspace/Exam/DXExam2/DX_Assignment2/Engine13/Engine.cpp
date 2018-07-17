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

// ȸ�� ���� �����.
float rot = 0.01f;
float pi = 3.141592f;
float doublePI = pi * 2.0f;
void Engine::Update(double deltaTime)
{
	// ī�޶� ����.
	UpdateCamera();

	//// ��ü ȸ����Ű��.
	//RotateObject(deltaTime);
}

void Engine::RotateObject(double deltaTime)
{
	// ���� ��� �ʱ�ȭ.
	world = XMMatrixIdentity();

	/*float rotX = XMConvertToRadians(-90.0f);
	XMMATRIX rotationXBase = XMMatrixRotationX(rotX);*/

	// X��/Z�� ȸ�� ��� �����.
	XMMATRIX rotationX = XMMatrixRotationX(rotX);
	XMMATRIX rotationZ = XMMatrixRotationZ(rotZ);

	// �̵�.
	//XMMATRIX translataion = XMMatrixTranslation(0.0, 1.0f, 3.0f);

	// ���� ��Ŀ� ȸ�� ��� �� ����.
	world = rotationX * rotationZ;

	// ���̴� ������ۿ� ������ ������ ����.
	cbData.world = XMMatrixTranspose(world);
	cbData.view = XMMatrixTranspose(view);
	cbData.projection = XMMatrixTranspose(projection);

	// ��� ���� ����.
	pDeviceContext->UpdateSubresource(cbBuffer, 0, 0, &cbData, 0, 0);

	// ���������� ���ε�.
	pDeviceContext->VSSetConstantBuffers(0, 1, &cbBuffer);
}

void Engine::Render()
{
	// ��� �����. (����� �����).
	float color[4] = { 0.2f, 0.3f, 0.47f, 1.0f };
	pDeviceContext->ClearRenderTargetView(pRenderTargetView, color);

	// ����/���ٽ� �����.
	pDeviceContext->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	
	// �޽� �׸���.
	MeshDraw();

	// ���� ��ü. (����� <-> ����Ʈ����).
	pSwapChain->Present(0, 0);
}

void Engine::MeshDraw()
{
	if (meshes.size() < 1)
		return;

	// �޽� �׸���.
	for (int ix = 0; ix < meshes.size(); ++ix)
	{
		// ���̴� ���ε�.
		if (!BindShader(&meshes[ix])) return;

		// ���� ���� ���ε�.
		BindVertexBuffer(&meshes[ix]);

		// �ε��� ���� ���ε�.
		BindIndexBuffer(&meshes[ix]);

		// �Է� ���̾ƿ� ���ε�.
		BindInputLayout(&meshes[ix]);

		// �⺻ �������� ���� (TRIANGLELIST�� ����).
		pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		// �ؽ�ó ���ε�.
		meshes[ix].pixelShader.SetTextures(pDeviceContext);

		// ���÷� ������Ʈ ���ε�.
		meshes[ix].pixelShader.SetSamplerState(pDeviceContext);

		// �����Ͷ����� ������Ʈ ���ε�.
		meshes[ix].vertexShader.BindRasterizerState(pDeviceContext);

		// WVP ������� ����.
		UpdateWVPBuffer(&meshes[ix]);

		// ����Ʈ ������� ����.
		UpdateLightCB();

		// �޽� �׸���.
		meshes[ix].DrawMesh(pDeviceContext);
	}
}