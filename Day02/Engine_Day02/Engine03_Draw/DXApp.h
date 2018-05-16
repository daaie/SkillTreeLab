#pragma once
#include <Windows.h>
#include "DXUtil.h"
#include <xnamath.h>
//directx 기반 프레임워크 xna
//11부터는 윈도우쪾이나 xna 수학lib헤더를 쓰는게 일반적임

class DXApp
{
public:
	struct Vertex
	{
		XMFLOAT3 position ; //xna기반 x,y,z 구조체임.

		//참고
		//float color[4]; 
		//XMFLOAT4 color
		//float r,g,b,a
		//color texture(UV)-얘는 인덱스로 표현함..찾아봐 등
		Vertex(){}
		Vertex(float x, float y, float z) : position(x, y, z) {} //초기화
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
	// 구버전은 디바이스가 하나. 11 얘는 디바이스가 두개. 
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
	
protected:
	bool InitWindow();
	bool InitDirect3D();
	bool InitScene();
};

