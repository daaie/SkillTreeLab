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

//회전각도 저장용.
float rot = 0.0f;
void Engine::Update()
{
	//double tempTime = timeGetTime() / 1000;

	// 회전 각도 변경.
	//rot += (2.0f*3.14f)*tempTime;
	//rot += XMConvertToRadians(0.1f); 라디안으로 바꿔주는 함수.
	// rad2deg deg2rad 같은 함수.
	rot += 0.00005f;

	if (rot > 3.14f * 2.0f) //2pi가 되면 원점
		rot = 0.0f;

	// 회전 행렬
	XMMATRIX rotation = XMMatrixRotationY(rot);
	// 이동 행렬
	XMMATRIX translation = XMMatrixTranslation(3.0f,1.0f,0.0f);
	// 스케일 행렬
	XMMATRIX scale = XMMatrixScaling(3.0f, 1.5f, 1.0f);

	// 월드 행렬 갱신

	world = XMMatrixIdentity();
	world = rotation;
	//world = translation * rotation;
	//world = scale * rotation *translation;
	//곱셈 순서가 중요함 순서에 따라 변환순서가 바뀜

	// 월드 회전시켰다고 월드행렬에 누적되지 않는다. 회전 행렬을 계속 늘려줘야한다.

	CBPerObject cbData;
	cbData.world = XMMatrixTranspose(world);
	cbData.view = XMMatrixTranspose(view);
	cbData.projection = XMMatrixTranspose(projection);

	// 상수 버퍼 갱신. 서브 리소스가 하나라서 0,0 하면되는데 두번쨰 세번쨰 인자 신경써줘야함
	// pDeviceContext->UpdateSubresource(cbBuffer, 0, 0, &cbData, 0, 0);

	// 파이프라인 바인딩
	// pDeviceContext->VSSetConstantBuffers(0, 1, &cbBuffer); -> 바인딩은 되어있으니까 리소스가 바구이ㅓ도 잘함
}

void Engine::Render()
{
	float color[4] = { 0.0f,0.4f,0.6f,1.0f }; // 마지막은 알파 값
	//배경 지우기
	pDeviceContext->ClearRenderTargetView(pRenderTargetView, color);

	//정점개수,정점읽기시작하는 위치
	//pDeviceContext->Draw(3, 0);
	//인덱스버퍼에있는 6개를 처음부터 다ㅏ읽겠다
	// pDeviceContext->DrawIndexed(6, 0, 0);
	pDeviceContext->DrawIndexed(nIndices,0,0);

	//버퍼 교체 ( 백버퍼 <-> 프론트버퍼 )
	pSwapChain->Present(0, 0);
	
}
