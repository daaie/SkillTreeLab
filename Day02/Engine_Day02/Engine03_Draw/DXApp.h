#pragma once
#include <Windows.h>
#include "DXUtil.h"
#include <xnamath.h>
//directx ��� �����ӿ�ũ xna
//11���ʹ� ������U�̳� xna ����lib����� ���°� �Ϲ�����

class DXApp
{
public:
	struct Vertex
	{
		XMFLOAT3 position ; //xna��� x,y,z ����ü��.

		//����
		//float color[4]; 
		//XMFLOAT4 color
		//float r,g,b,a
		//color texture(UV)-��� �ε����� ǥ����..ã�ƺ� ��
		Vertex(){}
		Vertex(float x, float y, float z) : position(x, y, z) {} //�ʱ�ȭ
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
	// �������� ����̽��� �ϳ�. 11 ��� ����̽��� �ΰ�. 
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

