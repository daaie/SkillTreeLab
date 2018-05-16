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
	
	// �����ӿ�ũ �޼ҵ�.
	virtual bool Init(); 
	virtual void Update() = 0; 
	virtual void Render() = 0; 

	virtual LRESULT MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam); 

protected:

	// Win32 ��� ����.
	HWND		hwnd; //Ÿ�Ը𸣸� f12Ÿ�� ������ ��..
	HINSTANCE	hinstance;
	UINT		clientWidth;
	UINT		clientHeight;
	LPCSTR		appTitle; 
	DWORD		wndStyle; 

	// DirectX 11 ��� ����
	// �������� ����̽��� �ϳ�. 11 ��� ����̽��� �ΰ�. 
	ID3D11Device*	pDevice; // ������ �������� �׷���ī�忡 �������
	ID3D11DeviceContext* pDeviceContext; // ������ ����

	IDXGISwapChain *pSwapChain; // ����ü�� - �����, ����Ʈ ���� 
	ID3D11RenderTargetView * pRenderTargetView; // ����� �׸���. �׸�����ϸ� ����ü���� ���Ѵ�.
	
protected:
	// ������ �ʱ�ȭ.
	bool InitWindow();
	bool InitDirect3D();
};

