#pragma once
#include <Windows.h>
#include "DXUtil.h"
#include <xnamath.h>

class DXApp
{
public:
	struct Vertex
	{
		XMFLOAT4 position ; 
		XMFLOAT4 color;

		Vertex(){}
		Vertex(float x, float y, float z, float w, float r, float g, float b, float a) : position(x, y, z, w), color(r, g, b, a){}
	};
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

	ID3D11Device*	pDevice; // ������ �������� �׷���ī�忡 �������
	ID3D11DeviceContext* pDeviceContext; // ������ ����

	IDXGISwapChain *pSwapChain; // ����ü�� - �����, ����Ʈ ���� 
	ID3D11RenderTargetView * pRenderTargetView; // ����� �׸���. �׸�����ϸ� ����ü���� ���Ѵ�.

	ID3D11Buffer *vertexBuffer; // ������ ������ ����.
	ID3D11VertexShader *vertexShader; // ���� ���̴�.
	ID3D11PixelShader *pixelShader; // �ȼ� ���̴�. - �ﰢ�� ����
	ID3DBlob *vertextShaderBuffer; // ���̴� ������ �ؼ� �� ������ ����.
	ID3DBlob *pixelShaderBuffer;
	ID3D11InputLayout * vertexInputLayout; // ���� �Է� ���̾ƿ�,  ��ġ, ����, UV1, UV2(��ְ�) ��
	
protected:
	bool InitWindow();
	bool InitDirect3D();
	bool InitScene();
};

