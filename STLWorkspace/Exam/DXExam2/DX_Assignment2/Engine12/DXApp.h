#pragma once

#include <Windows.h>
#include "DXUtil.h"
#include "GameTimer.h"
#include "FBXLoader.h"
#include "ShaderUtil.h"

class DXApp
{
public:

	// 라이트 처리를 위한 구조체 정의.
	struct CBLight
	{
		// 빛의 위치(3차원 위치 값).
		XMVECTOR lightPosition;
		XMVECTOR cameraPosition;
	};

	DXApp(HINSTANCE hinstance);
	virtual ~DXApp();

	// 메인 메시지 루프.
	int Run();

	// 프레임워크 메소드.
	virtual bool Init();
	virtual void Update(double deltaTime) = 0;
	virtual void Render() = 0;
	virtual LRESULT MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

protected:

	// Win32 멤버 변수.
	HWND		hwnd;
	HINSTANCE	hinstance;
	UINT		clientWidth;
	UINT		clientHeight;
	LPCWSTR		appTitle;
	DWORD		wndStyle;

	// DirectX 11 멤버 변수.
	ID3D11Device*			pDevice;			// 렌더링 외의 다른 업무 전담.
	ID3D11DeviceContext*	pDeviceContext;		// 렌더링 전담.
	IDXGISwapChain*			pSwapChain;			// 스왑 체인.
	ID3D11RenderTargetView* pRenderTargetView;	// 렌더 타겟.

	ID3D11InputLayout*	vertexInputLayout;		// 정점 입력 레이아웃.

	// 공간 변환 행렬.
	XMMATRIX world;
	XMMATRIX view;
	XMMATRIX projection;

	// 뷰행렬 만들 때 필요한 값.
	XMVECTOR cameraPos;
	XMVECTOR cameraTarget;
	XMVECTOR cameraUp;

	// 게임 타이머 변수 추가.
	GameTimer gameTimer;

	std::vector<Shape::Mesh>	meshes;

	// WVP 상수 버퍼용 변수.
	ShaderUtil::CBPerObject		cbData;

	// WVP 상수 버퍼.
	ID3D11Buffer*				cbBuffer;

	// 라이트 상수 버퍼용 변수.
	CBLight						cbLight;

	// 라이트 버퍼.
	ID3D11Buffer*				lightBuffer;

	// 뎁스/스텐실 버퍼 관련 변수.
	ID3D11DepthStencilView*		depthStencilView;
	ID3D11Texture2D*			depthStencilBuffer;

	// 다이렉트 입력 관련 변수.
	IDirectInputDevice8*		diKeyboard;		// 키보드 입력 처리용.
	IDirectInputDevice8*		diMouse;			// 마우스 입력 처리용.

	// 마우스의 지난 입력 값 저장용.
	DIMOUSESTATE				mouseLastState;
	LPDIRECTINPUT8				directInput;

	// 물체 회전용 변수.
	float rotX = 0.0f;
	float rotZ = 0.0f;

	XMMATRIX rotationX;
	XMMATRIX rotationZ;

	// 카메라 이동/회전 관련 변수.
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

	// Win32 메소드.
	// 윈도우 초기화.
	bool InitWindow();

	// DirectX 초기화.
	bool InitDirect3D();

	// 장면 초기화.
	bool InitScene();

	// 메쉬 정보 생성 및 설정.
	void InitMeshInfo();

	// 쉐이더 컴파일2.
	bool ShaderCompile(Shape::Mesh* mesh);

	// 쉐이더 바인딩.
	bool BindShader(Shape::Mesh* mesh);

	// 정점 버퍼 초기화2.
	bool InitVertexBuffer(Shape::Mesh* mesh);

	// 정점 버퍼 바인딩.
	void BindVertexBuffer(Shape::Mesh* mesh);

	// 인덱스 버퍼 초기화2.
	bool InitIndexBuffer(Shape::Mesh* mesh);

	// 인덱스 버퍼 바인딩.
	void BindIndexBuffer(Shape::Mesh* mesh);

	// 입력 레이아웃 설정2.
	bool InitInputLayout(Shape::Mesh* mesh);

	// 입력 레이아웃 바인딩.
	void BindInputLayout(Shape::Mesh* mesh);

	// 뷰포트 설정.
	void SetViewport();

	// 공간변환 행렬 설정.
	void InitTransformMatrix(Shape::Mesh* mesh);

	void InitWorldMatrix(Shape::Mesh* mesh);

	void InitViewMatrix(Shape::Mesh* mesh);

	void UpdateViewMatrix(Shape::Mesh* mesh);

	void InitProjectionMatrix(Shape::Mesh* mesh);

	bool InitWVPBuffer(Shape::Mesh * mesh);

	void UpdateWVPBuffer(Shape::Mesh* mesh);

	// 라이트 정보 생성.
	void InitLightCB();

	// 라이트 정보 갱신.
	void UpdateLightCB();

	// 라이트 정보 바인딩.
	void BindLightCB();

	// 뎁스/스텐실 버퍼 설정.
	bool InitDepthStencilBuffer();

	// 입력 장치 초기화.
	bool InitDirectInput(HINSTANCE hinstance);

	// 입력 감지.
	void DetectInput(double deltaTime);

	// 카메라 업데이트.
	void UpdateCamera();
};