#include "DXApp.h"

// 메시지 프로시져 설정을 우회하는데 사용하기 위해 생성.
DXApp* g_pApp = NULL;

LRESULT CALLBACK MainMsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	// g_pApp 변수가 설정됐으면 MsgProc 사용.
	if (g_pApp) return g_pApp->MsgProc(hwnd, msg, wParam, lParam);

	// 설정되지 않으면 윈도우 기본 프로시저 함수 호출.
	else return DefWindowProc(hwnd, msg, wParam, lParam);
}

DXApp::DXApp(HINSTANCE hinstance)
{
	// 멤버 변수 초기화.
	hwnd = NULL;
	this->hinstance = hinstance;
	clientWidth = 1280;
	clientHeight = 800;
	appTitle = L"Engine21 - MultipleObjects";
	wndStyle = WS_OVERLAPPEDWINDOW;
	g_pApp = this;

	// DX 관련 멤버 변수 초기화.
	pDevice = NULL;
	pDeviceContext = NULL;
	pSwapChain = NULL;
	pRenderTargetView = NULL;

	vertexInputLayout = NULL;

	LPCSTR fbxName = "HeroTPP.fbx";

	ShaderUtil::Texture diffuseMap;
	diffuseMap.fileName = L"T_Chr_FPS_D.png";
	
	ShaderUtil::Texture normalMap;
	normalMap.fileName = L"T_Chr_FPS_N.png";

	ShaderUtil::Texture cubeMap;
	cubeMap.fileName = L"skymap.dds";

	diffuseMesh.fbxName = fbxName;
	diffuseMesh.vertexShader = ShaderUtil::VertexShader(L"Effects.fx", "VS_Main", "vs_5_0");
	diffuseMesh.pixelShader = ShaderUtil::PixelShader(L"Effects.fx", "PS_Main", "ps_5_0");
	diffuseMesh.pixelShader.pTextures.push_back(diffuseMap);
	diffuseMesh.position = XMFLOAT3(-100.0f, -90.0f, 0.0f);
	diffuseMesh.rotation = XMFLOAT3(-90.0f, 180.0f, 0.0f);
	meshes.push_back(diffuseMesh);

	normalMesh.fbxName = fbxName;
	normalMesh.vertexShader = ShaderUtil::VertexShader(L"NormalVS.fx", "main", "vs_5_0");
	normalMesh.pixelShader = ShaderUtil::PixelShader(L"NormalPS.fx", "main", "ps_5_0");
	normalMesh.pixelShader.pTextures.push_back(diffuseMap);
	normalMesh.pixelShader.pTextures.push_back(normalMap);
	normalMesh.position = XMFLOAT3(0.0f, -90.0f, 0.0f);
	normalMesh.rotation = XMFLOAT3(-90.0f, 180.0f, 0.0f);

	meshes.push_back(normalMesh);

	cubeMesh.fbxName = fbxName;
	cubeMesh.vertexShader = ShaderUtil::VertexShader(L"CubeMapVS.fx", "main", "vs_5_0");
	cubeMesh.pixelShader = ShaderUtil::PixelShader(L"CubeMapPS.fx", "main", "ps_5_0");
	cubeMesh.pixelShader.pTextures.push_back(diffuseMap);
	cubeMesh.pixelShader.pTextures.push_back(normalMap);
	cubeMesh.pixelShader.pTextures.push_back(cubeMap);
	cubeMesh.position = XMFLOAT3(100.0f, -90.0f, 0.0f);
	cubeMesh.rotation = XMFLOAT3(-90.0f, 180.0f, 0.0f);
	
	meshes.push_back(cubeMesh);
}

DXApp::~DXApp()
{
	// 메모리 해제.
	Memory::SafeRelease(pDevice);
	Memory::SafeRelease(pDeviceContext);
	Memory::SafeRelease(pSwapChain);
	Memory::SafeRelease(pRenderTargetView);

	Memory::SafeRelease(vertexInputLayout);

	Memory::SafeRelease(cbBuffer);

	Memory::SafeRelease(pDiffuseMapTexture);
	Memory::SafeRelease(pDiffuseMapSamplerState);
	
	Memory::SafeRelease(rsState);

	// 블렌드 스테이트 해제.
	Memory::SafeRelease(alphaBlendState);

	// 뎁스/스텐실 버퍼 관련 리소스 해제.
	Memory::SafeRelease(depthStencilView);
	Memory::SafeRelease(depthStencilBuffer);

	normalMesh.Release(); 
	cubeMesh.Release();
}

int DXApp::Run()
{
	// 메인 메시지 처리.
	MSG msg;
	// 0으로 초기화.
	ZeroMemory(&msg, sizeof(MSG));
	while (msg.message != WM_QUIT)
	{
		// 메시지 처리.
		// GetMessage();
		if (PeekMessage(&msg, NULL, NULL, NULL, PM_REMOVE))
		{
			// 메시지 해석.
			TranslateMessage(&msg);
			// 메시지 전달.
			DispatchMessage(&msg);
		}

		// DirectX 렌더링 루프.
		else
		{
			// 타이머 실행.
			gameTimer.UpdateTimer();

			// 프레임 시간 계산.
			float deltaTime = gameTimer.DeltaTime();

			// 입력 감지.
			DetectInput(deltaTime);
			
			// Update Scene.
			Update(deltaTime);

			// Render Scene.
			Render();
		}
	}

	return (int)msg.wParam;
}

bool DXApp::Init()
{
	// 윈도우 초기화.
	//if (!InitWindow())
	//if (false == InitWindow())
	if (InitWindow() == false)
		return false;

	// DirectX 초기화.
	if (InitDirect3D() == false)
		return false;

	// 장면 초기화.
	if (InitScene() == false)
		return false;

	// 다이렉트 입력 초기화.
	if (InitDirectInput(hinstance) == false)
		return false;

	return true;
}

bool DXApp::InitWindow()
{
	// 윈도우 클래스 값 생성/설정.
	WNDCLASSEX wc;
	// 구조체 초기화.
	ZeroMemory(&wc, sizeof(WNDCLASSEX));

	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.hbrBackground = NULL;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.lpszClassName = L"WindowClass";
	wc.lpszMenuName = NULL;
	wc.lpfnWndProc = MainMsgProc;

	// 윈도우 클래스 등록.
	if (RegisterClassEx(&wc) == false)
		return false;

	// 윈도우 생성.
	hwnd = CreateWindow(L"WindowClass", appTitle,
		wndStyle, 0, 0, clientWidth, clientHeight,
		NULL, NULL, hinstance, NULL);
	// 생성 실패하면 종료.
	if (hwnd == NULL)
	{
		return false;
	}

	// 창 띄우기.
	ShowWindow(hwnd, SW_SHOW);

	return true;
}

bool DXApp::InitDirect3D()
{
	// 스왑 체인 정보 설정.
	DXGI_SWAP_CHAIN_DESC swapDesc;
	ZeroMemory(&swapDesc, sizeof(DXGI_SWAP_CHAIN_DESC));
	swapDesc.BufferCount = 1;
	swapDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapDesc.OutputWindow = hwnd;
	swapDesc.Windowed = true;
	//swapDesc.Windowed = false;		// 풀 스크린 모드로 실행.
	swapDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH; // 화면 크기 변경되면 갱신.
	swapDesc.BufferDesc.Width = clientWidth;
	swapDesc.BufferDesc.Height = clientHeight;
	swapDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapDesc.BufferDesc.RefreshRate.Numerator = 60;
	swapDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapDesc.SampleDesc.Count = 1;
	swapDesc.SampleDesc.Quality = 0;

	// 장치 및 스왑체인 생성.
	HRESULT hr;
	hr = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, NULL, NULL, NULL, D3D11_SDK_VERSION, &swapDesc, &pSwapChain, 
		&pDevice, NULL, &pDeviceContext);

	// 오류 확인.
	//if (SUCCEEDED(hr)) // S_OK
	if (FAILED(hr))		 // E_FAIL
	{
		MessageBox(NULL, L"장치 생성 실패", L"오류", MB_OK);
		return false;
	}

	// 2D 텍스처 생성 (렌더 타겟 용).
	ID3D11Texture2D* pBackBufferTexture;
	hr = pSwapChain->GetBuffer(NULL, __uuidof(ID3D11Texture2D),
		(void**)&pBackBufferTexture);

	if (FAILED(hr))		 // E_FAIL
	{
		MessageBox(NULL, L"스왑 버퍼 생성 실패", L"오류", MB_OK);
		return false;
	}

	// 렌더 타켓 뷰 생성.
	hr = pDevice->CreateRenderTargetView(pBackBufferTexture, NULL,
		&pRenderTargetView);
	
	if (FAILED(hr))		 // E_FAIL
	{
		MessageBox(NULL, L"렌더 타겟 뷰 생성 실패", L"오류", MB_OK);
		return false;
	}
	
	// 뎁스/스텐실 버퍼 설정.
	if (!InitDepthStencilBuffer())
		return false;

	// 렌더 타켓 뷰 바인딩. 파이프 라인에 연결.
	pDeviceContext->OMSetRenderTargets(1, &pRenderTargetView, depthStencilView);

	// 리소스 해제.
	Memory::SafeRelease(pBackBufferTexture);

	return true;
}

bool DXApp::ShaderCompile(Shape::Mesh* mesh)
{
	// 정점 쉐이더 컴파일/바인딩.
	if (!mesh->vertexShader.CompileShader()) return false;
	if (!mesh->vertexShader.BindShader(pDevice, pDeviceContext)) return false;

	// 픽셀 쉐이더 컴파일/바인딩.
	if (!mesh->pixelShader.CompileShader()) return false;
	if (!mesh->pixelShader.BindShader(pDevice, pDeviceContext)) return false;

	return true;
}

bool DXApp::InitVertexBuffer(Shape::Mesh* mesh)
{
	if (FAILED(FBXLoader::LoadFBX(mesh->fbxName, mesh))) return false;
	if (!mesh->CreateVertexBuffer(pDevice)) return false;
	mesh->BindVertexBuffer(pDeviceContext);

	return true;
}

bool DXApp::InitIndexBuffer(Shape::Mesh* mesh)
{
	if (!mesh->CreateIndexBuffer(pDevice)) return false;
	mesh->BindIndexBuffer(pDeviceContext);

	return true;
}

bool DXApp::InitInputLayout(Shape::Mesh * mesh)
{
	if (!mesh->CreateInputLayout(pDevice)) return false;
	mesh->BindInputLayout(pDeviceContext);

	return true;
}

void DXApp::SetViewport()
{
	// 뷰포트 정보 설정.
	D3D11_VIEWPORT viewport;
	ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = clientWidth;
	viewport.Height = clientHeight;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;

	// 뷰포트 설정.
	pDeviceContext->RSSetViewports(1, &viewport);
}

void DXApp::InitTransformMatrix(Shape::Mesh* mesh)
{
	// World x View x Projection;
	// 월드 행렬. (위치/회전/스케일).
	world = XMMatrixIdentity();
	XMMATRIX rotationX = XMMatrixRotationX(XMConvertToRadians(mesh->rotation.x));
	XMMATRIX rotationY = XMMatrixRotationY(XMConvertToRadians(mesh->rotation.y));
	XMMATRIX translation = XMMatrixTranslation(mesh->position.x, mesh->position.y, mesh->position.z);

	world *= rotationX * rotationY * translation;

	// 카메라 위치.
	cameraPos = XMVectorSet(0.0f, 0.0f, -200.0f, 0.0f);

	// 카메라가 바라보는 위치.
	cameraTarget = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);

	// 카메라 위 방향.
	cameraUp = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	// 뷰 행렬.
	view = XMMatrixLookAtLH(cameraPos, cameraTarget, cameraUp);

	// 시야각.
	float fovY = 75.0f * 3.14 / 180.0f;

	// 투영 행렬.
	projection = XMMatrixPerspectiveFovLH(fovY, (float)clientWidth / clientHeight, 1.0f, 1000.0f);

	// 상수 버퍼 데이터 설정 및 파이프라인 바인딩.
	// 전치행렬을 만들어서 데이터 채우기.
	// DX는 행우선 / HLSL은 열우선.
	mesh->vertexShader.cbWVP.world = XMMatrixTranspose(world);
	mesh->vertexShader.cbWVP.view = XMMatrixTranspose(view);
	mesh->vertexShader.cbWVP.projection = XMMatrixTranspose(projection);

	// 버퍼 디스크립터.
	D3D11_BUFFER_DESC cbDesc;
	ZeroMemory(&cbDesc, sizeof(D3D11_BUFFER_DESC));
	cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbDesc.ByteWidth = sizeof(CBPerObject);
	cbDesc.CPUAccessFlags = 0;
	cbDesc.MiscFlags = 0;
	cbDesc.Usage = D3D11_USAGE_DEFAULT;

	// 서브 리소스 생성.
	D3D11_SUBRESOURCE_DATA cbData;
	ZeroMemory(&cbData, sizeof(D3D11_SUBRESOURCE_DATA));
	//cbData.pSysMem = &data;
	cbData.pSysMem = &mesh->vertexShader.cbWVP;

	// 상수 버퍼 생성.
	HRESULT hr = pDevice->CreateBuffer(&cbDesc, &cbData, &cbBuffer);

	// 파이프라인에 바인딩.
	pDeviceContext->VSSetConstantBuffers(0, 1, &cbBuffer);
}

bool DXApp::InitScene()
{
	// 뷰포트 설정.
	SetViewport();

	// 래스터라이저 상태 설정.
	if (SetRasterizerState() == false)
		return false;

	// 블렌드 스테이트 설정.
	//SetBlendState();

	return true;
}

bool DXApp::SetBlendState()
{
	// 블렌드 스테이드 서술자 설정.
	D3D11_BLEND_DESC blendDesc;
	ZeroMemory(&blendDesc, sizeof(D3D11_BLEND_DESC));
	blendDesc.RenderTarget[0].BlendEnable = true;
	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blendDesc.RenderTarget[0].DestBlend =
		D3D11_BLEND_INV_SRC_ALPHA;
	blendDesc.RenderTarget[0].BlendOp =
		D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].RenderTargetWriteMask
		= D3D11_COLOR_WRITE_ENABLE_ALL;
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	// 블렌드 스테이트 생성.
	pDevice->CreateBlendState(&blendDesc, &alphaBlendState);

	// 블렌드 스테이트 바인딩.
	pDeviceContext->OMSetBlendState(alphaBlendState, 0, 0xffffffff);

	return true;
}

void DXApp::InitLightCB()
{
	// 상수 버퍼로 전달할 라이트 값 설정.
	CBLight cbLight;
	ZeroMemory(&cbLight, sizeof(cbLight));
	cbLight.lightPosition = XMVectorSet(500.0f, 500.0f, -500.0f, 1.0f);
	cbLight.cameraPosition = cameraPos;

	// 버텍스 버퍼 서술자(Descriptor).
	D3D11_BUFFER_DESC vbDesc;
	ZeroMemory(&vbDesc, sizeof(D3D11_BUFFER_DESC));
	vbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	vbDesc.ByteWidth = sizeof(CBLight);
	vbDesc.CPUAccessFlags = 0;
	vbDesc.MiscFlags = 0;
	vbDesc.Usage = D3D11_USAGE_DEFAULT;

	// 서브 리소스 생성.
	D3D11_SUBRESOURCE_DATA vbData;
	ZeroMemory(&vbData, sizeof(D3D11_SUBRESOURCE_DATA));
	vbData.pSysMem = &cbLight;

	// 버퍼 생성.
	ID3D11Buffer* lightBuffer;
	pDevice->CreateBuffer(&vbDesc, &vbData, &lightBuffer);

	// 정점 파이프라인에 바인딩.
	pDeviceContext->VSSetConstantBuffers(1, 1, &lightBuffer);
}

bool DXApp::InitDepthStencilBuffer()
{
	// 버퍼 서술자 설정.
	D3D11_TEXTURE2D_DESC depthStencilDesc;
	ZeroMemory(&depthStencilDesc, sizeof(D3D11_TEXTURE2D_DESC));
	depthStencilDesc.Width = clientWidth;
	depthStencilDesc.Height = clientHeight;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilDesc.SampleDesc.Count = 1;
	depthStencilDesc.SampleDesc.Quality = 0;
	depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
	depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStencilDesc.CPUAccessFlags = 0;
	depthStencilDesc.MiscFlags = 0;

	// 뎁스/스텐실 버퍼 생성.
	HRESULT hr;
	hr = pDevice->CreateTexture2D(&depthStencilDesc, NULL, &depthStencilBuffer);
	if (FAILED(hr))
	{
		MessageBox(NULL, L"뎁스/스텐실 버퍼 생성 실패", L"오류", MB_OK);
		return false;
	}

	// 뎁스/스텐실 뷰 생성.
	hr = pDevice->CreateDepthStencilView(depthStencilBuffer, NULL, &depthStencilView);
	if (FAILED(hr))
	{
		MessageBox(NULL, L"뎁스/스텐실 뷰 생성 실패", L"오류", MB_OK);
		return false;
	}

	return true;
}

bool DXApp::InitDirectInput(HINSTANCE hinstance)
{
	// 입력 초기화.
	HRESULT hr = DirectInput8Create(hinstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&directInput, NULL);
	if (FAILED(hr))
	{
		MessageBox(NULL, L"다이렉트 입력 초기화 실패", L"오류", MB_OK);
		return false;
	}

	// 장치 초기화.
	hr = directInput->CreateDevice(GUID_SysKeyboard, &diKeyboard, NULL);
	if (FAILED(hr))
	{
		MessageBox(NULL, L"키보드 장치 초기화 실패", L"오류", MB_OK);
		return false;
	}

	hr = directInput->CreateDevice(GUID_SysMouse, &diMouse, NULL);
	if (FAILED(hr))
	{
		MessageBox(NULL, L"마우스 장치 초기화 실패", L"오류", MB_OK);
		return false;
	}

	// 데이터 포맷 설정.
	hr = diKeyboard->SetDataFormat(&c_dfDIKeyboard);
	if (FAILED(hr))
	{
		MessageBox(NULL, L"키보드 포맷 설정 실패", L"오류", MB_OK);
		return false;
	}

	// 협력 레벨 설정.
	hr = diKeyboard->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	if (FAILED(hr))
	{
		MessageBox(NULL, L"키보드 협력 레벨 설정 실패", L"오류", MB_OK);
		return false;
	}

	hr = diMouse->SetDataFormat(&c_dfDIMouse);
	if (FAILED(hr))
	{
		MessageBox(NULL, L"마우스 데이터 설정 실패", L"오류", MB_OK);
		return false;
	}

	hr = diMouse->SetCooperativeLevel(hwnd, DISCL_EXCLUSIVE | DISCL_FOREGROUND | DISCL_NOWINKEY);
	if (FAILED(hr))
	{
		MessageBox(NULL, L"마우스 협력 레벨 설정 실패", L"오류", MB_OK);
		return false;
	}

	return true;
}

void DXApp::DetectInput(double deltaTime)
{
	// 키보드 / 마우스 입력 감지를 위한 값 선언.
	DIMOUSESTATE currentMouseState;
	BYTE keyboardState[256];

	// 입력 획득.
	diKeyboard->Acquire();
	diMouse->Acquire();

	// 키보드/마우스 장치 상태 얻어오기.
	diKeyboard->GetDeviceState(sizeof(keyboardState), (LPVOID)&keyboardState);
	diMouse->GetDeviceState(sizeof(currentMouseState), &currentMouseState);

	// 키 입력 확인.
	if (keyboardState[DIK_ESCAPE] & 0x80)
		PostMessage(hwnd, WM_DESTROY, 0, 0);

	if (keyboardState[DIK_LEFT] & 0x80) moveLeftRight -= 1.0f * deltaTime;
	if (keyboardState[DIK_RIGHT] & 0x80) moveLeftRight += 1.0f * deltaTime;

	if (keyboardState[DIK_UP] & 0x80) moveBackForward += 1.0f * deltaTime;
	if (keyboardState[DIK_DOWN] & 0x80) moveBackForward -= 1.0f * deltaTime;

	// 마우스 입력.
	if (currentMouseState.lX != mouseLastState.lX ||
		currentMouseState.lY != mouseLastState.lY)
	{
		cameraYaw += currentMouseState.lX * 0.001f;
		cameraPitch += currentMouseState.lY * 0.001f;
	}

	// 이번 프레임의 마우스 입력을 저장.
	mouseLastState = currentMouseState;

	//// 카메라 갱신.
	//UpdateCamera();
}

void DXApp::UpdateCamera()
{
	// 카메라 회전.
	cameraRotationMatrix = XMMatrixRotationRollPitchYaw(cameraPitch, cameraYaw, 0.0f);

	// 카메라가 보는 방향 설정.
	cameraTarget = XMVector3TransformCoord(defaultForward, cameraRotationMatrix);
	cameraTarget = XMVector3Normalize(cameraTarget);

	XMMATRIX rotateYTemp;
	rotateYTemp = XMMatrixRotationY(cameraYaw);

	// 카메라 오른쪽 방향 설정 (회전 값 적용해서 방향 설정).
	cameraRight = XMVector3TransformCoord(defaultRight, rotateYTemp);
	cameraUp = XMVector3TransformCoord(cameraUp, rotateYTemp);
	cameraForward = XMVector3TransformCoord(defaultForward, rotateYTemp);

	// 카메라 위치 이동.
	cameraPos += moveLeftRight * cameraRight;
	cameraPos += moveBackForward * cameraForward;

	// 사용한 입력 초기화.
	moveLeftRight = 0.0f;
	moveBackForward = 0.0f;

	// 카메라 바라보는 방향 벡터에 위치 적용.
	cameraTarget = cameraPos + cameraTarget;

	// 뷰 행렬 갱신.
	view = XMMatrixLookAtLH(cameraPos, cameraTarget, cameraUp);
}

bool DXApp::SetRasterizerState()
{
	// 래스터라이저 서술자 설정.
	D3D11_RASTERIZER_DESC rsDesc;
	ZeroMemory(&rsDesc, sizeof(D3D11_RASTERIZER_DESC));
	rsDesc.CullMode = D3D11_CULL_BACK;		// 뒷면 안그리기.
	rsDesc.FillMode = D3D11_FILL_SOLID;	// 와이퍼 프레임으로 그리기.

	HRESULT hr;
	// 래스터라이저 상태 생성.
	hr = pDevice->CreateRasterizerState(&rsDesc, &rsState);
	if (FAILED(hr))
		return false;

	// 바인딩.
	pDeviceContext->RSSetState(rsState);

	return true;
}

LRESULT DXApp::MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_KEYDOWN:
		// ESC 키 눌렸는지 확인.
		if (wParam == VK_ESCAPE)
		{
			// 창 닫기 여부를 묻는 메시지 박스 생성.
			if (MessageBox(NULL, L"종료하시겠습니까?", L"종료", MB_YESNO | MB_ICONQUESTION) == IDYES)
			{
				// Yes(예) 키 눌리면 창 삭제.
				DestroyWindow(this->hwnd);
			}
		}
		return 0;

		// 창이 삭제되면 실행.
	case WM_DESTROY:
		// 프로그램 종료 메시지 호출.
		PostQuitMessage(0);
		return 0;

	default: return DefWindowProc(hwnd, msg, wParam, lParam);
	}
}