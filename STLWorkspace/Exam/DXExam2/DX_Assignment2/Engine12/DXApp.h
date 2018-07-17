#pragma once

#include <Windows.h>
#include "DXUtil.h"
#include "GameTimer.h"
#include "FBXLoader.h"
#include "ShaderUtil.h"

class DXApp
{
public:

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

	ID3D11InputLayout*	vertexInputLayout;		// ���� �Է� ���̾ƿ�.

	// ���� ��ȯ ���.
	XMMATRIX world;
	XMMATRIX view;
	XMMATRIX projection;

	// ����� ���� �� �ʿ��� ��.
	XMVECTOR cameraPos;
	XMVECTOR cameraTarget;
	XMVECTOR cameraUp;

	// ���� Ÿ�̸� ���� �߰�.
	GameTimer gameTimer;

	std::vector<Shape::Mesh>	meshes;

	// WVP ��� ���ۿ� ����.
	ShaderUtil::CBPerObject		cbData;

	// WVP ��� ����.
	ID3D11Buffer*				cbBuffer;

	// ����Ʈ ��� ���ۿ� ����.
	CBLight						cbLight;

	// ����Ʈ ����.
	ID3D11Buffer*				lightBuffer;

	// ����/���ٽ� ���� ���� ����.
	ID3D11DepthStencilView*		depthStencilView;
	ID3D11Texture2D*			depthStencilBuffer;

	// ���̷�Ʈ �Է� ���� ����.
	IDirectInputDevice8*		diKeyboard;		// Ű���� �Է� ó����.
	IDirectInputDevice8*		diMouse;			// ���콺 �Է� ó����.

	// ���콺�� ���� �Է� �� �����.
	DIMOUSESTATE				mouseLastState;
	LPDIRECTINPUT8				directInput;

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

	float cameraMoveSpeed = 300.0f;
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

	// �޽� ���� ���� �� ����.
	void InitMeshInfo();

	// ���̴� ������2.
	bool ShaderCompile(Shape::Mesh* mesh);

	// ���̴� ���ε�.
	bool BindShader(Shape::Mesh* mesh);

	// ���� ���� �ʱ�ȭ2.
	bool InitVertexBuffer(Shape::Mesh* mesh);

	// ���� ���� ���ε�.
	void BindVertexBuffer(Shape::Mesh* mesh);

	// �ε��� ���� �ʱ�ȭ2.
	bool InitIndexBuffer(Shape::Mesh* mesh);

	// �ε��� ���� ���ε�.
	void BindIndexBuffer(Shape::Mesh* mesh);

	// �Է� ���̾ƿ� ����2.
	bool InitInputLayout(Shape::Mesh* mesh);

	// �Է� ���̾ƿ� ���ε�.
	void BindInputLayout(Shape::Mesh* mesh);

	// ����Ʈ ����.
	void SetViewport();

	// ������ȯ ��� ����.
	void InitTransformMatrix(Shape::Mesh* mesh);

	void InitWorldMatrix(Shape::Mesh* mesh);

	void InitViewMatrix(Shape::Mesh* mesh);

	void UpdateViewMatrix(Shape::Mesh* mesh);

	void InitProjectionMatrix(Shape::Mesh* mesh);

	bool InitWVPBuffer(Shape::Mesh * mesh);

	void UpdateWVPBuffer(Shape::Mesh* mesh);

	// ����Ʈ ���� ����.
	void InitLightCB();

	// ����Ʈ ���� ����.
	void UpdateLightCB();

	// ����Ʈ ���� ���ε�.
	void BindLightCB();

	// ����/���ٽ� ���� ����.
	bool InitDepthStencilBuffer();

	// �Է� ��ġ �ʱ�ȭ.
	bool InitDirectInput(HINSTANCE hinstance);

	// �Է� ����.
	void DetectInput(double deltaTime);

	// ī�޶� ������Ʈ.
	void UpdateCamera();
};