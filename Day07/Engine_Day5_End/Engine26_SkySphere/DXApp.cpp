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
	appTitle = L"Engine25 - Toon Shader";
	wndStyle = WS_OVERLAPPEDWINDOW;
	g_pApp = this;

	// DX 관련 멤버 변수 초기화.
	pDevice = NULL;
	pDeviceContext = NULL;
	pSwapChain = NULL;
	pRenderTargetView = NULL;

	vertexInputLayout = NULL;
}

DXApp::~DXApp()
{
	// 풀 스크린 모드 해제.
	pSwapChain->SetFullscreenState(false, NULL);

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

	Memory::SafeRelease(pSkymapResourceView);
	Memory::SafeRelease(pDSLessEqual);
	Memory::SafeRelease(pRSCullNone);
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
	if (!InitWindow()) return false;

	// DirectX 초기화.
	if (!InitDirect3D()) return false;

	// 장면 초기화.
	if (!InitScene()) return false;

	// 다이렉트 입력 초기화.
	if (!InitDirectInput(hinstance)) return false;

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
	if (hwnd == NULL) return false;

	// 창 띄우기.
	ShowWindow(hwnd, SW_SHOW);

	// 창 업데이트하기.
	UpdateWindow(hwnd);

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
	if (!mesh->vertexShader.CreateShader(pDevice)) return false;

	// 픽셀 쉐이더 컴파일/바인딩.
	if (!mesh->pixelShader.CompileShader()) return false;
	if (!mesh->pixelShader.CreateShader(pDevice)) return false;

	return true;
}

bool DXApp::BindShader(Shape::Mesh * mesh)
{
	if (!mesh->vertexShader.BindShader(pDeviceContext)) return false;
	if (!mesh->pixelShader.BindShader(pDeviceContext)) return false;

	return true;
}

bool DXApp::InitVertexBuffer(Shape::Mesh* mesh)
{
	if (FAILED(FBXLoader::LoadFBX(mesh->fbxName, mesh))) return false;
	if (!mesh->CreateVertexBuffer(pDevice)) return false;

	return true;
}

void DXApp::BindVertexBuffer(Shape::Mesh * mesh)
{
	mesh->BindVertexBuffer(pDeviceContext);
}

bool DXApp::InitIndexBuffer(Shape::Mesh* mesh)
{
	if (!mesh->CreateIndexBuffer(pDevice)) return false;

	return true;
}

void DXApp::BindIndexBuffer(Shape::Mesh * mesh)
{
	mesh->BindIndexBuffer(pDeviceContext);
}

bool DXApp::InitInputLayout(Shape::Mesh * mesh)
{
	if (!mesh->CreateInputLayout(pDevice)) return false;

	return true;
}

void DXApp::BindInputLayout(Shape::Mesh * mesh)
{
	mesh->BindInputLayout(pDeviceContext);
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
	// WVP 행렬 초기화.
	InitWorldMatrix(mesh);
	InitViewMatrix(mesh);
	InitProjectionMatrix(mesh);

	// 상수 버퍼 데이터 설정 및 파이프라인 바인딩.
	// 전치행렬을 만들어서 데이터 채우기.
	// DX는 행우선 / HLSL은 열우선.
	mesh->vertexShader.cbWVP.world = XMMatrixTranspose(world);
	mesh->vertexShader.cbWVP.view = XMMatrixTranspose(view);
	mesh->vertexShader.cbWVP.projection = XMMatrixTranspose(projection);

	// 상수버퍼 생성.
	InitWVPBuffer(mesh);

	// 파이프라인에 바인딩.
	pDeviceContext->VSSetConstantBuffers(0, 1, &cbBuffer);
}

void DXApp::InitWorldMatrix(Shape::Mesh * mesh)
{
	world = XMMatrixIdentity();
	XMMATRIX scale = XMMatrixScaling(mesh->scale.x, mesh->scale.y, mesh->scale.z);
	XMMATRIX rotationX = XMMatrixRotationX(XMConvertToRadians(mesh->rotation.x));
	XMMATRIX rotationY = XMMatrixRotationY(XMConvertToRadians(mesh->rotation.y));
	XMMATRIX rotationZ = XMMatrixRotationZ(XMConvertToRadians(mesh->rotation.z));
	XMMATRIX rotation = rotationZ * rotationX * rotationY;
	XMMATRIX translation = XMMatrixTranslation(mesh->position.x, mesh->position.y, mesh->position.z);

	world *= scale * rotation * translation;
}

void DXApp::InitViewMatrix(Shape::Mesh * mesh)
{
	// 카메라 위치.
	cameraPos = XMVectorSet(0.0f, 0.0f, -200.0f, 0.0f);

	// 카메라가 바라보는 위치.
	cameraTarget = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);

	// 카메라 위 방향.
	cameraUp = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	// 뷰 행렬.
	view = XMMatrixLookAtLH(cameraPos, cameraTarget, cameraUp);
}

void DXApp::UpdateViewMatrix(Shape::Mesh * mesh)
{
	mesh->vertexShader.cbWVP.view = XMMatrixTranspose(view);
	pDeviceContext->UpdateSubresource(cbBuffer, 0, 0, &mesh->vertexShader.cbWVP, 0, 0);
}

void DXApp::InitProjectionMatrix(Shape::Mesh * mesh)
{
	// 시야각.
	float fovY = 75.0f * 3.14 / 180.0f;

	// 투영 행렬.
	projection = XMMatrixPerspectiveFovLH(fovY, (float)clientWidth / (float)clientHeight, 1.0f, 100.0f);
}

bool DXApp::InitWVPBuffer(Shape::Mesh * mesh)
{
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
	cbData.pSysMem = &mesh->vertexShader.cbWVP;

	// 상수 버퍼 생성.
	if (FAILED(pDevice->CreateBuffer(&cbDesc, &cbData, &cbBuffer)))
	{
		OutputDebugString(L"WVP 상수버퍼 생성 실패");
		OutputDebugString(L"\n");
		return false;
	}

	return true;
}

void DXApp::UpdateWVPBuffer(Shape::Mesh* mesh)
{
	InitWorldMatrix(mesh);
	mesh->vertexShader.cbWVP.world = XMMatrixTranspose(world);
	mesh->vertexShader.cbWVP.view = XMMatrixTranspose(view);
	mesh->vertexShader.cbWVP.projection = XMMatrixTranspose(projection);

	pDeviceContext->UpdateSubresource(cbBuffer, 0, 0, &mesh->vertexShader.cbWVP, 0, 0);
	
	// 파이프라인에 바인딩.
	pDeviceContext->VSSetConstantBuffers(0, 1, &cbBuffer);
}

bool DXApp::InitScene()
{
	// 메쉬 정보 설정.
	InitMeshInfo();

	// -------------------- New -------------------- //
	if (!ShaderCompile(&sphereMesh)) return false;
	if (!InitVertexBuffer(&sphereMesh)) return false;
	if (!InitIndexBuffer(&sphereMesh)) return false;
	if (!InitInputLayout(&sphereMesh)) return false;
	pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// 텍스처 로드.
	D3DX11_IMAGE_LOAD_INFO loadSMInfo;
	//ZeroMemory(&loadSMInfo, sizeof(D3DX11_IMAGE_LOAD_INFO));
	loadSMInfo.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;

	ID3D11Texture2D* smTexture = 0;
	HRESULT hr = D3DX11CreateTextureFromFile(pDevice, sphereMesh.pixelShader.pTextures[0].fileName,
		&loadSMInfo, NULL, (ID3D11Resource**)&smTexture, NULL);

	D3D11_TEXTURE2D_DESC smTextureDesc;
	ZeroMemory(&smTextureDesc, sizeof(D3D11_TEXTURE2D_DESC));
	smTexture->GetDesc(&smTextureDesc);

	D3D11_SHADER_RESOURCE_VIEW_DESC smViewDesc;
	ZeroMemory(&smViewDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
	smViewDesc.Format = smTextureDesc.Format;
	smViewDesc.ViewDimension = D3D10_SRV_DIMENSION_TEXTURECUBE;
	smViewDesc.TextureCube.MipLevels = smTextureDesc.MipLevels;
	smViewDesc.TextureCube.MostDetailedMip = 0;

	hr = pDevice->CreateShaderResourceView(smTexture, &smViewDesc,
		&sphereMesh.pixelShader.pTextures[0].pTextureResource);

	if (!sphereMesh.pixelShader.CreateSamplerState(pDevice)) return false;
	InitTransformMatrix(&sphereMesh);
	// -------------------- New -------------------- //

	for (int ix = 0; ix < meshes.size(); ++ix)
	{
		// 쉐이더 컴파일/오브젝트 생성.
		if (!ShaderCompile(&meshes[ix])) return false;

		// 정점 버퍼 생성.
		if (!InitVertexBuffer(&meshes[ix])) return false;

		// 인덱스 버퍼 생성.
		if (!InitIndexBuffer(&meshes[ix])) return false;

		// 입력 레이아웃 생성.
		if (!InitInputLayout(&meshes[ix])) return false;

		// 기본 토폴로지 설정 (TRIANGLELIST로 설정).
		pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		// 텍스처 로드.
		if (!meshes[ix].pixelShader.LoadTextures(pDevice)) return false;

		// 샘플러 스테이트 생성.
		if (!meshes[ix].pixelShader.CreateSamplerState(pDevice)) return false;

		// 공간 변환.
		InitTransformMatrix(&meshes[ix]);

		// 라이트 상수 버퍼 생성.
		InitLightCB();

		// 라이트 상수 버퍼 바인딩.
		BindLightCB();
	}

	// 뷰포트 설정.
	SetViewport();

	// 블렌드 스테이트 설정.
	//SetBlendState();

	// 래스터라이저 상태 설정.
	if (!SetRasterizerState()) return false;

	// --------------- New --------------- //
	rsDesc.CullMode = D3D11_CULL_NONE;
	hr = pDevice->CreateRasterizerState(&rsDesc, &pRSCullNone);

	D3D11_DEPTH_STENCIL_DESC dssDesc;
	ZeroMemory(&dssDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));
	dssDesc.DepthEnable = true;
	dssDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dssDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;

	pDevice->CreateDepthStencilState(&dssDesc, &pDSLessEqual);
	// --------------- New --------------- //

	return true;
}

bool DXApp::SetBlendState()
{
	// 블렌드 스테이드 서술자 설정.
	D3D11_BLEND_DESC blendDesc;
	ZeroMemory(&blendDesc, sizeof(D3D11_BLEND_DESC));
	blendDesc.RenderTarget[0].BlendEnable = true;
	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
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
	cbLight;
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
	pDevice->CreateBuffer(&vbDesc, &vbData, &lightBuffer);
}

void DXApp::UpdateLightCB()
{
	pDeviceContext->UpdateSubresource(lightBuffer, 0, 0, &cbLight, 0, 0);
	pDeviceContext->VSSetConstantBuffers(1, 1, &lightBuffer);
}

void DXApp::BindLightCB()
{
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

	if (keyboardState[DIK_LEFT] & 0x80 || keyboardState[DIK_A] & 0x80) moveLeftRight -= 1.0f * cameraMoveSpeed * deltaTime;
	if (keyboardState[DIK_RIGHT] & 0x80 || keyboardState[DIK_D] & 0x80) moveLeftRight += 1.0f * cameraMoveSpeed * deltaTime;

	if (keyboardState[DIK_UP] & 0x80 || keyboardState[DIK_W] & 0x80) moveBackForward += 1.0f  * cameraMoveSpeed * deltaTime;
	if (keyboardState[DIK_DOWN] & 0x80 || keyboardState[DIK_S] & 0x80) moveBackForward -= 1.0f  * cameraMoveSpeed * deltaTime;

	// 마우스 입력.
	if (currentMouseState.lX != mouseLastState.lX ||
		currentMouseState.lY != mouseLastState.lY)
	{
		cameraYaw += currentMouseState.lX * 0.001f;
		cameraPitch += currentMouseState.lY * 0.001f;
	}

	// 이번 프레임의 마우스 입력을 저장.
	mouseLastState = currentMouseState;
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
	//D3D11_RASTERIZER_DESC rsDesc;
	ZeroMemory(&rsDesc, sizeof(D3D11_RASTERIZER_DESC));
	rsDesc.CullMode = D3D11_CULL_NONE;		// 뒷면 안그리기.
	rsDesc.FillMode = D3D11_FILL_SOLID;	// 와이퍼 프레임으로 그리기.

	HRESULT hr;
	// 래스터라이저 상태 생성.
	hr = pDevice->CreateRasterizerState(&rsDesc, &rsState);
	if (FAILED(hr))
	{
		OutputDebugString(L"래스터라이저 상태 생성 실패");
		OutputDebugString(L"\n");
		return false;
	}

	// 바인딩.
	pDeviceContext->RSSetState(rsState);

	return true;
}

void DXApp::InitMeshInfo()
{
	LPCSTR fbxNameTPP = "Resources//Model//HeroTPP.fbx";
	LPCSTR fbxNameSphere = "Resources//Model//sphere.fbx";
	LPCSTR fbxNamePlane = "Resources//Model//plane.fbx";

	ShaderUtil::Texture diffuseMap;
	diffuseMap.fileName = L"Resources//Textures//T_Chr_FPS_D.png";

	ShaderUtil::Texture normalMap;
	normalMap.fileName = L"Resources//Textures//T_Chr_FPS_N.png";

	ShaderUtil::Texture cubeMap;
	cubeMap.fileName = L"Resources//Textures//skybox2.dds";

	ShaderUtil::Texture grassTexture;
	grassTexture.fileName = L"Resources//Textures//T_Ground_Grass_D.png";

	// ---------------------- New ---------------------- //
	// 구체 메쉬.
	sphereMesh;
	sphereMesh.fbxName = fbxNameSphere;
	sphereMesh.vertexShader = ShaderUtil::VertexShader(L"Shaders//SkyBoxVS.fx", "main", "vs_5_0");
	sphereMesh.pixelShader = ShaderUtil::PixelShader(L"Shaders//SkyBoxPS.fx", "main", "ps_5_0");
	sphereMesh.pixelShader.pTextures.push_back(cubeMap);
	sphereMesh.position = XMFLOAT3(0.0f, 0.0f, 100.0f);
	sphereMesh.rotation = XMFLOAT3(0.0f, 0.0f, 0.0f);
	sphereMesh.scale = XMFLOAT3(1.0f, 1.0f, 1.0f);
	// ---------------------- New ---------------------- //

	// 바닥 메쉬.
	Shape::Mesh planeMesh;
	planeMesh.fbxName = fbxNamePlane;
	planeMesh.vertexShader = ShaderUtil::VertexShader(L"Shaders//LabertianDiffuseVS.fx", "main", "vs_5_0");
	planeMesh.pixelShader = ShaderUtil::PixelShader(L"Shaders//LabertianDiffusePS.fx", "main", "ps_5_0");
	planeMesh.pixelShader.pTextures.push_back(grassTexture);
	planeMesh.position = XMFLOAT3(0.0f, -120.0f, -30.0f);
	planeMesh.rotation = XMFLOAT3(-90.0f, 0.0f, 0.0f);
	planeMesh.scale = XMFLOAT3(1000.0f, 1000.0f, 1.0f);
	meshes.push_back(planeMesh);

	// 메쉬 (디퓨즈맵 적용).
	Shape::Mesh diffuseMesh;
	diffuseMesh.fbxName = fbxNameTPP;
	diffuseMesh.vertexShader = ShaderUtil::VertexShader(L"Shaders//LabertianDiffuseVS.fx", "main", "vs_5_0");
	diffuseMesh.pixelShader = ShaderUtil::PixelShader(L"Shaders//LabertianDiffusePS.fx", "main", "ps_5_0");
	diffuseMesh.pixelShader.pTextures.push_back(diffuseMap);
	diffuseMesh.position = XMFLOAT3(-450.0f, -120.0f, -30.0f);
	diffuseMesh.rotation = XMFLOAT3(-90.0f, 180.0f, 0.0f);
	meshes.push_back(diffuseMesh);

	// 메쉬 (디퓨즈맵 적용).
	Shape::Mesh halfLambertMesh;
	halfLambertMesh.fbxName = fbxNameTPP;
	halfLambertMesh.vertexShader = ShaderUtil::VertexShader(L"Shaders//HalfLabertianDiffuseVS.fx", "main", "vs_5_0");
	halfLambertMesh.pixelShader = ShaderUtil::PixelShader(L"Shaders//HalfLabertianDiffusePS.fx", "main", "ps_5_0");
	halfLambertMesh.pixelShader.pTextures.push_back(diffuseMap);
	halfLambertMesh.position = XMFLOAT3(-300.0f, -120.0f, -30.0f);
	halfLambertMesh.rotation = XMFLOAT3(-90.0f, 180.0f, 0.0f);
	meshes.push_back(halfLambertMesh);

	// 메쉬 (디퓨즈맵 + 툰 셰이딩 적용).
	Shape::Mesh toonMesh;
	toonMesh.fbxName = fbxNameTPP;
	toonMesh.vertexShader = ShaderUtil::VertexShader(L"Shaders//ToonVS.fx", "main", "vs_5_0");
	toonMesh.pixelShader = ShaderUtil::PixelShader(L"Shaders//ToonPS.fx", "main", "ps_5_0");
	toonMesh.pixelShader.pTextures.push_back(diffuseMap);
	toonMesh.position = XMFLOAT3(-150.0f, -120.0f, -30.0f);
	toonMesh.rotation = XMFLOAT3(-90.0f, 180.0f, 0.0f);
	meshes.push_back(toonMesh);

	// 메쉬 (디퓨즈맵 + 스페큘러 + 노멀맵 적용).
	Shape::Mesh normalMesh;
	normalMesh.fbxName = fbxNameTPP;
	normalMesh.vertexShader = ShaderUtil::VertexShader(L"Shaders//NormalVS.fx", "main", "vs_5_0");
	normalMesh.pixelShader = ShaderUtil::PixelShader(L"Shaders//NormalPS.fx", "main", "ps_5_0");
	normalMesh.pixelShader.pTextures.push_back(diffuseMap);
	normalMesh.pixelShader.pTextures.push_back(normalMap);
	normalMesh.position = XMFLOAT3(0.0f, -120.0f, -30.0f);
	normalMesh.rotation = XMFLOAT3(-90.0f, 180.0f, 0.0f);

	meshes.push_back(normalMesh);

	// 메쉬 (디퓨즈맵 + 스페큘러 + 노멀맵 + 큐브맵 적용).
	Shape::Mesh cubeMesh;
	cubeMesh.fbxName = fbxNameTPP;
	cubeMesh.vertexShader = ShaderUtil::VertexShader(L"Shaders//CubeMapVS.fx", "main", "vs_5_0");
	cubeMesh.pixelShader = ShaderUtil::PixelShader(L"Shaders//CubeMapPS.fx", "main", "ps_5_0");
	cubeMesh.pixelShader.pTextures.push_back(diffuseMap);
	cubeMesh.pixelShader.pTextures.push_back(normalMap);
	cubeMesh.pixelShader.pTextures.push_back(cubeMap);
	cubeMesh.position = XMFLOAT3(150.0f, -120.0f, -30.0f);
	cubeMesh.rotation = XMFLOAT3(-90.0f, 180.0f, 0.0f);

	meshes.push_back(cubeMesh);
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