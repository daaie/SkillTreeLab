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
		//XMFLOAT4 color;
		XMFLOAT2 texCoord;

		Vertex(){}
		Vertex(float x, float y, float z, float u, float v) : position(x, y, z), texCoord(u,v){}
		Vertex(XMFLOAT3 pos, XMFLOAT2 texCoord) : position(pos), texCoord(texCoord) {}
	};

	// ��� ���ۿ� ����ü ����
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

	ID3D11Buffer *indexBuffer; // �ε�������
	
	XMMATRIX world;
	XMMATRIX view;
	XMMATRIX projection;

	// ����� ���� �� �ʿ��� ��
	XMVECTOR cameraPos;
	XMVECTOR cameraTarget;
	XMVECTOR cameraUp;

	// ��� ����
	ID3D11Buffer *cbBuffer;

	// �ε��� ����
	int nIndices;
	// ���� ����
	int nVertices;

	// �ؽ��� ������� / ���÷�(�ؽ��� �д°�) ����(state) �ʿ� ��׵��� ��� ������.. 
	ID3D11ShaderResourceView* pTexture; // �ؽ��� �������.
	ID3D11SamplerState* pTextureSamplerState;

	// �����Ͷ����� ���� ���� .. ������ ����ǵ� �װ� �����Ͷ��������� �ϴ°ǵ� ����/
	ID3D11RasterizerState* rsState;


protected:
	bool InitWindow();
	bool InitDirect3D();
	bool InitScene();
	// �ؽ�ó �ε�
	bool InitTexture();
	// ������ ������ ���¼���
	bool SetRasterizerState();
};

