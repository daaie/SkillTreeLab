#pragma once
#include <Windows.h>
#include "DXUtil.h"
#include <xnamath.h>
#include "GameTimer.h"
#include <vector>

class DXApp
{
public:

	// ���� ����ü ����.
	struct Vertex
	{
		XMFLOAT3 position;
		XMFLOAT3 normal;

		Vertex() { }
	};

	// ��� ���ۿ� ����ü ����.
	struct CBPerObject
	{
		XMMATRIX world;
		XMMATRIX view;
		XMMATRIX projection;
	};

	// ����Ʈ ó���� ���� ����ü ����.
	struct CBLight
	{
		// ���� ��ġ(3���� ��ġ ��).
		XMVECTOR lightPosition;
		XMVECTOR cameraPosition;
	};

	DXApp(HINSTANCE hinstance);
	virtual ~DXApp();

	// ���� �޽��� ����.
	int Run();

	// �����ӿ�ũ �޼ҵ�.
	virtual bool Init();
	virtual void Update(double deltaTime) = 0;
	virtual void Render() = 0;
	virtual LRESULT MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

protected:

	// Win32 ��� ����.
	HWND		hwnd;
	HINSTANCE	hinstance;
	UINT		clientWidth;
	UINT		clientHeight;
	LPCWSTR		appTitle;
	DWORD		wndStyle;

	// DirectX 11 ��� ����.
	ID3D11Device*			pDevice;			// ������ ���� �ٸ� ���� ����.
	ID3D11DeviceContext*	pDeviceContext;		// ������ ����.
	IDXGISwapChain*			pSwapChain;			// ���� ü��.
	ID3D11RenderTargetView* pRenderTargetView;	// ���� Ÿ��.

	ID3D11Buffer*		vertexBuffer;			// ���� ����.
	ID3D11VertexShader* vertexShader;			// ���� ���̴�(���̴�).
	ID3D11PixelShader*  pixelShader;			// �ȼ� ���̴�.
	ID3DBlob*			vertexShaderBuffer;		// ���� ���̴� ����.
	ID3DBlob*			pixelShaderBuffer;		// �ȼ� ���̴� ����.
	ID3D11InputLayout*	vertexInputLayout;		// ���� �Է� ���̾ƿ�.

	ID3D11Buffer*		indexBuffer;			// �ε��� ����.

	// ���� ��ȯ ���.
	XMMATRIX world;
	XMMATRIX view;
	XMMATRIX projection;

	// ����� ���� �� �ʿ��� ��.
	XMVECTOR cameraPos;
	XMVECTOR cameraTarget;
	XMVECTOR cameraUp;

	// ��� ����.
	ID3D11Buffer* cbBuffer;

	// �ε��� ����.
	int nIndices;

	// ���� ����.
	int nVertices;

	// �ؽ�ó/���÷� ����(State).
	ID3D11ShaderResourceView*	pTexture;
	ID3D11SamplerState*			pTextureSamplerState;

	// �����Ͷ����� ���� ����.
	ID3D11RasterizerState*		rsState;

	// ���� ������Ʈ(����) ����.
	ID3D11BlendState*			alphaBlendState;

	// ���� Ÿ�̸� ���� �߰�.
	GameTimer gameTimer;

	// ���� �迭.
	std::vector<Vertex> pVertices;

	// FBX Manager ����.
	FbxManager* fbxManager;

	// ����/���ٽ� ���� ���� ����.
	ID3D11DepthStencilView* depthStencilView;
	ID3D11Texture2D* depthStencilBuffer;

	// ���̷�Ʈ �Է� ���� ����.
	IDirectInputDevice8* diKeyboard;		// Ű���� �Է� ó����.
	IDirectInputDevice8* diMouse;			// ���콺 �Է� ó����.

	// ���콺�� ���� �Է� �� �����.
	DIMOUSESTATE mouseLastState;
	LPDIRECTINPUT8 directInput;

	// ��ü ȸ���� ����.
	float rotX = 0.0f;
	float rotZ = 0.0f;

	XMMATRIX rotationX;
	XMMATRIX rotationZ;

	// ī�޶� �̵�/ȸ�� ���� ����.
	XMVECTOR defaultForward = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	XMVECTOR defaultRight = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);

	XMVECTOR cameraForward = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	XMVECTOR cameraRight = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);

	XMMATRIX cameraRotationMatrix;

	float moveLeftRight = 0.0f;
	float moveBackForward = 0.0f;

	float cameraYaw = 0.0f;
	float cameraPitch = 0.0f;

protected:

	// Win32 �޼ҵ�. 
	// ������ �ʱ�ȭ.
	bool InitWindow();

	// DirectX �ʱ�ȭ.
	bool InitDirect3D();

	// ��� �ʱ�ȭ.
	bool InitScene();

	// �����Ͷ����� ���� ���� �޼ҵ�.
	bool SetRasterizerState();

	// ���̴� ������.
	bool ShaderCompile();

	// ���� ���� �ʱ�ȭ.
	void InitVertexBuffer();

	// �ε��� ���� �ʱ�ȭ
	void InitIndexBuffer();

	// �Է� ���̾ƿ� ����.
	bool InitInputLayout();

	// ����Ʈ ����.
	void SetViewport();

	// ������ȯ ��� ����.
	void InitTransformMatrix();

	// �ؽ�ó �ε� �޼ҵ�.
	bool InitTexture();

	// ���� ������Ʈ ����.
	bool SetBlendState();

	// ����Ʈ ���� ���� �� ���ε�.
	void InitLightCB();

	// ����/���ٽ� ���� ����.
	bool InitDepthStencilBuffer();

	// �Է� ��ġ �ʱ�ȭ.
	bool InitDirectInput(HINSTANCE hinstance);
	// �Է� ����.
	void DetectInput(double deltaTime);

	// ī�޶� ������Ʈ.
	void UpdateCamera();

	// FBX �ε� �޼ҵ�.
	HRESULT LoadFBX(const char* fileName, std::vector<Vertex>* pOutVertex);

	// ���� ��� �о���� �޼ҵ�.
	XMFLOAT3 ReadNormal(FbxMesh* pMesh, int vertexIndex, int vertexCounter);
};