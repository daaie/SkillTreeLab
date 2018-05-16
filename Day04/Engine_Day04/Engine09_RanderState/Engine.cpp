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

//ȸ������ �����.
float rot = 0.0f;
void Engine::Update()
{
	//double tempTime = timeGetTime() / 1000;

	// ȸ�� ���� ����.
	//rot += (2.0f*3.14f)*tempTime;
	//rot += XMConvertToRadians(0.1f); �������� �ٲ��ִ� �Լ�.
	// rad2deg deg2rad ���� �Լ�.
	rot += 0.00005f;

	if (rot > 3.14f * 2.0f) //2pi�� �Ǹ� ����
		rot = 0.0f;

	// ȸ�� ���
	XMMATRIX rotation = XMMatrixRotationY(rot);
	// �̵� ���
	XMMATRIX translation = XMMatrixTranslation(3.0f,1.0f,0.0f);
	// ������ ���
	XMMATRIX scale = XMMatrixScaling(3.0f, 1.5f, 1.0f);

	// ���� ��� ����

	world = XMMatrixIdentity();
	world = rotation;
	//world = translation * rotation;
	//world = scale * rotation *translation;
	//���� ������ �߿��� ������ ���� ��ȯ������ �ٲ�

	// ���� ȸ�����״ٰ� ������Ŀ� �������� �ʴ´�. ȸ�� ����� ��� �÷�����Ѵ�.

	CBPerObject cbData;
	cbData.world = XMMatrixTranspose(world);
	cbData.view = XMMatrixTranspose(view);
	cbData.projection = XMMatrixTranspose(projection);

	// ��� ���� ����. ���� ���ҽ��� �ϳ��� 0,0 �ϸ�Ǵµ� �ι��� ������ ���� �Ű�������
	// pDeviceContext->UpdateSubresource(cbBuffer, 0, 0, &cbData, 0, 0);

	// ���������� ���ε�
	// pDeviceContext->VSSetConstantBuffers(0, 1, &cbBuffer); -> ���ε��� �Ǿ������ϱ� ���ҽ��� �ٱ��̤õ� ����
}

void Engine::Render()
{
	float color[4] = { 0.0f,0.4f,0.6f,1.0f }; // �������� ���� ��
	//��� �����
	pDeviceContext->ClearRenderTargetView(pRenderTargetView, color);

	//��������,�����б�����ϴ� ��ġ
	//pDeviceContext->Draw(3, 0);
	//�ε������ۿ��ִ� 6���� ó������ �٤��аڴ�
	// pDeviceContext->DrawIndexed(6, 0, 0);
	pDeviceContext->DrawIndexed(nIndices,0,0);

	//���� ��ü ( ����� <-> ����Ʈ���� )
	pSwapChain->Present(0, 0);
	
}
