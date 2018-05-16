#pragma once
#include <Windows.h>
#include "DXUtil.h"

class DXApp
{
public:
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
	
protected:
	// 윈도우 초기화.
	bool InitWindow();
	bool InitDirect3D();
};

