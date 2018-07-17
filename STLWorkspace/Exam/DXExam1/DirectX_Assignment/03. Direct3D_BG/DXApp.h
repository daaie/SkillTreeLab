#pragma once
#include <Windows.h>
#include "DXUtil.h"

class DXApp
{
public:
	DXApp(HINSTANCE hinstance);
	virtual ~DXApp();

	int Run();

	virtual bool Init();
	virtual void Update() = 0;
	virtual void Render() = 0;
	virtual LRESULT MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

protected:

	HWND		hwnd;
	HINSTANCE	hinstance;
	UINT		clientWidth;
	UINT		clientHeight;
	LPCWSTR		appTitle;
	DWORD		wndStyle;

	// DirectX 11 ¸â¹ö º¯¼ö.
	ID3D11Device*		 pDevice;		
	ID3D11DeviceContext* pDeviceContext;	
	IDXGISwapChain*		 pSwapChain;	
	ID3D11RenderTargetView* pRenderTargetView;

protected:

	bool InitWindow();
	bool InitDirect3D();
};