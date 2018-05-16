#pragma once
#include <Windows.h>
#include "DXUtil.h"
#include <xnamath.h>

class DXApp
{
public:
	struct Vertex
	{
		XMFLOAT3 position ; 
		XMFLOAT4 color;

		Vertex(){}
		Vertex(float x, float y, float z, float r, float g, float b, float a) : position(x, y, z), color(r, g, b, a){}
		Vertex(XMFLOAT3 pos, XMFLOAT4 color) : position(pos), color(color) {}
	};

	// 상수 버퍼용 구조체 정의
	struct CBPerObject
	{
		XMMATRIX world;
		XMMATRIX view;
		XMMATRIX projection;
	};

	DXApp(HINSTANCE hinstance);
	virtual ~DXApp(); 

	// Main Message Loop .
	int Run();
	
	// 프레임워크 메소드.
	virtual bool Init(); 
	virtual void Update() = 0; 
	virtual void Render() = 0; 

	virtual LRESULT MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam); 

protected:

	// Win32 멤버 변수.
	HWND		hwnd; //타입모르면 f12타고 가ㅅㅓ 봐..
	HINSTANCE	hinstance;
	UINT		clientWidth;
	UINT		clientHeight;
	LPCSTR		appTitle; 
	DWORD		wndStyle; 

	// DirectX 11 멤버 변수

	ID3D11Device*	pDevice; // 렌더링 외적으로 그래픽카드에 명령전달
	ID3D11DeviceContext* pDeviceContext; // 렌더링 관련

	IDXGISwapChain *pSwapChain; // 스왑체인 - 백버퍼, 프론트 버퍼 
	ID3D11RenderTargetView * pRenderTargetView; // 여기다 그린다. 그리라고하면 스왑체인이 일한다.

	ID3D11Buffer *vertexBuffer; // 정점을 저장할 공간.
	ID3D11VertexShader *vertexShader; // 정점 셰이더.
	ID3D11PixelShader *pixelShader; // 픽셀 셰이더. - 삼각형 색상
	ID3DBlob *vertextShaderBuffer; // 셰이더 파일을 해석 후 데이터 공간.
	ID3DBlob *pixelShaderBuffer;
	ID3D11InputLayout * vertexInputLayout; // 정점 입력 레이아웃,  위치, 색상, UV1, UV2(노멀값) 등

	ID3D11Buffer *indexBuffer; // 인덱스버퍼
	
	XMMATRIX world;
	XMMATRIX view;
	XMMATRIX projection;

	// 뷰행렬 만들 때 필요한 값
	XMVECTOR cameraPos;
	XMVECTOR cameraTarget;
	XMVECTOR cameraUp;

	// 상수 버퍼
	ID3D11Buffer *cbBuffer;

protected:
	bool InitWindow();
	bool InitDirect3D();
	bool InitScene();
};

