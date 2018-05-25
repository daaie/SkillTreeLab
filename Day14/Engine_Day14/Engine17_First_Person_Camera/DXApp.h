#pragma once
#include <Windows.h>
#include "DXUtil.h"
#include <xnamath.h>
#include "GameTimer.h"
#include <vector>

class DXApp
{
public:

	// 정점 구조체 정의.
	struct Vertex
	{
		XMFLOAT3 position;
		XMFLOAT3 normal;

		Vertex() { }
	};

	// 상수 버퍼용 구조체 정의.
	struct CBPerObject
	{
		XMMATRIX world;
		XMMATRIX view;
		XMMATRIX projection;
	};

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

	ID3D11Buffer*		vertexBuffer;			// 정점 버퍼.
	ID3D11VertexShader* vertexShader;			// 정점 쉐이더(셰이더).
	ID3D11PixelShader*  pixelShader;			// 픽셀 쉐이더.
	ID3DBlob*			vertexShaderBuffer;		// 정점 쉐이더 버퍼.
	ID3DBlob*			pixelShaderBuffer;		// 픽셀 쉐이더 버퍼.
	ID3D11InputLayout*	vertexInputLayout;		// 정점 입력 레이아웃.

	ID3D11Buffer*		indexBuffer;			// 인덱스 버퍼.

	// 공간 변환 행렬.
	XMMATRIX world;
	XMMATRIX view;
	XMMATRIX projection;

	// 뷰행렬 만들 때 필요한 값.
	XMVECTOR cameraPos;
	XMVECTOR cameraTarget;
	XMVECTOR cameraUp;

	// 상수 버퍼.
	ID3D11Buffer* cbBuffer;

	// 인덱스 개수.
	int nIndices;

	// 정점 개수.
	int nVertices;

	// 텍스처/샘플러 상태(State).
	ID3D11ShaderResourceView*	pTexture;
	ID3D11SamplerState*			pTextureSamplerState;

	// 래스터라이저 상태 변수.
	ID3D11RasterizerState*		rsState;

	// 블렌드 스테이트(상태) 변수.
	ID3D11BlendState*			alphaBlendState;

	// 게임 타이머 변수 추가.
	GameTimer gameTimer;

	// 정점 배열.
	std::vector<Vertex> pVertices;

	// FBX Manager 변수.
	FbxManager* fbxManager;

	// 뎁스/스텐실 버퍼 관련 변수.
	ID3D11DepthStencilView* depthStencilView;
	ID3D11Texture2D* depthStencilBuffer;

	// 다이렉트 입력 관련 변수.
	IDirectInputDevice8* diKeyboard;		// 키보드 입력 처리용.
	IDirectInputDevice8* diMouse;			// 마우스 입력 처리용.

	// 마우스의 지난 입력 값 저장용.
	DIMOUSESTATE mouseLastState;
	LPDIRECTINPUT8 directInput;

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

	// 래스터라이저 상태 설정 메소드.
	bool SetRasterizerState();

	// 쉐이더 컴파일.
	bool ShaderCompile();

	// 정점 버퍼 초기화.
	void InitVertexBuffer();

	// 인덱스 버퍼 초기화
	void InitIndexBuffer();

	// 입력 레이아웃 설정.
	bool InitInputLayout();

	// 뷰포트 설정.
	void SetViewport();

	// 공간변환 행렬 설정.
	void InitTransformMatrix();

	// 텍스처 로드 메소드.
	bool InitTexture();

	// 블렌드 스테이트 설정.
	bool SetBlendState();

	// 라이트 정보 설정 및 바인딩.
	void InitLightCB();

	// 뎁스/스텐실 버퍼 설정.
	bool InitDepthStencilBuffer();

	// 입력 장치 초기화.
	bool InitDirectInput(HINSTANCE hinstance);
	// 입력 감지.
	void DetectInput(double deltaTime);

	// 카메라 업데이트.
	void UpdateCamera();

	// FBX 로드 메소드.
	HRESULT LoadFBX(const char* fileName, std::vector<Vertex>* pOutVertex);

	// 정점 노멀 읽어오는 메소드.
	XMFLOAT3 ReadNormal(FbxMesh* pMesh, int vertexIndex, int vertexCounter);
};