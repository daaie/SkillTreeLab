#include "DXApp.h"

// �޽��� ���ν��� ������ ��ȸ�ϴµ� ����ϱ� ���� ����.
DXApp* g_pApp = NULL;

LRESULT CALLBACK MainMsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	// g_pApp ������ ���������� MsgProc ���.
	if (g_pApp) return g_pApp->MsgProc(hwnd, msg, wParam, lParam);

	// �������� ������ ������ �⺻ ���ν��� �Լ� ȣ��.
	else return DefWindowProc(hwnd, msg, wParam, lParam);
}

DXApp::DXApp(HINSTANCE hinstance)
{
	// ��� ���� �ʱ�ȭ.
	hwnd = NULL;
	this->hinstance = hinstance;
	clientWidth = 1280;
	clientHeight = 800;
	appTitle = L"Engine21 - MultipleObjects";
	wndStyle = WS_OVERLAPPEDWINDOW;
	g_pApp = this;

	// DX ���� ��� ���� �ʱ�ȭ.
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
	// �޸� ����.
	Memory::SafeRelease(pDevice);
	Memory::SafeRelease(pDeviceContext);
	Memory::SafeRelease(pSwapChain);
	Memory::SafeRelease(pRenderTargetView);

	Memory::SafeRelease(vertexInputLayout);

	Memory::SafeRelease(cbBuffer);

	Memory::SafeRelease(pDiffuseMapTexture);
	Memory::SafeRelease(pDiffuseMapSamplerState);
	
	Memory::SafeRelease(rsState);

	// ���� ������Ʈ ����.
	Memory::SafeRelease(alphaBlendState);

	// ����/���ٽ� ���� ���� ���ҽ� ����.
	Memory::SafeRelease(depthStencilView);
	Memory::SafeRelease(depthStencilBuffer);

	normalMesh.Release(); 
	cubeMesh.Release();
}

int DXApp::Run()
{
	// ���� �޽��� ó��.
	MSG msg;
	// 0���� �ʱ�ȭ.
	ZeroMemory(&msg, sizeof(MSG));
	while (msg.message != WM_QUIT)
	{
		// �޽��� ó��.
		// GetMessage();
		if (PeekMessage(&msg, NULL, NULL, NULL, PM_REMOVE))
		{
			// �޽��� �ؼ�.
			TranslateMessage(&msg);
			// �޽��� ����.
			DispatchMessage(&msg);
		}

		// DirectX ������ ����.
		else
		{
			// Ÿ�̸� ����.
			gameTimer.UpdateTimer();

			// ������ �ð� ���.
			float deltaTime = gameTimer.DeltaTime();

			// �Է� ����.
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
	// ������ �ʱ�ȭ.
	//if (!InitWindow())
	//if (false == InitWindow())
	if (InitWindow() == false)
		return false;

	// DirectX �ʱ�ȭ.
	if (InitDirect3D() == false)
		return false;

	// ��� �ʱ�ȭ.
	if (InitScene() == false)
		return false;

	// ���̷�Ʈ �Է� �ʱ�ȭ.
	if (InitDirectInput(hinstance) == false)
		return false;

	return true;
}

bool DXApp::InitWindow()
{
	// ������ Ŭ���� �� ����/����.
	WNDCLASSEX wc;
	// ����ü �ʱ�ȭ.
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

	// ������ Ŭ���� ���.
	if (RegisterClassEx(&wc) == false)
		return false;

	// ������ ����.
	hwnd = CreateWindow(L"WindowClass", appTitle,
		wndStyle, 0, 0, clientWidth, clientHeight,
		NULL, NULL, hinstance, NULL);
	// ���� �����ϸ� ����.
	if (hwnd == NULL)
	{
		return false;
	}

	// â ����.
	ShowWindow(hwnd, SW_SHOW);

	return true;
}

bool DXApp::InitDirect3D()
{
	// ���� ü�� ���� ����.
	DXGI_SWAP_CHAIN_DESC swapDesc;
	ZeroMemory(&swapDesc, sizeof(DXGI_SWAP_CHAIN_DESC));
	swapDesc.BufferCount = 1;
	swapDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapDesc.OutputWindow = hwnd;
	swapDesc.Windowed = true;
	//swapDesc.Windowed = false;		// Ǯ ��ũ�� ���� ����.
	swapDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH; // ȭ�� ũ�� ����Ǹ� ����.
	swapDesc.BufferDesc.Width = clientWidth;
	swapDesc.BufferDesc.Height = clientHeight;
	swapDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapDesc.BufferDesc.RefreshRate.Numerator = 60;
	swapDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapDesc.SampleDesc.Count = 1;
	swapDesc.SampleDesc.Quality = 0;

	// ��ġ �� ����ü�� ����.
	HRESULT hr;
	hr = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, NULL, NULL, NULL, D3D11_SDK_VERSION, &swapDesc, &pSwapChain, 
		&pDevice, NULL, &pDeviceContext);

	// ���� Ȯ��.
	//if (SUCCEEDED(hr)) // S_OK
	if (FAILED(hr))		 // E_FAIL
	{
		MessageBox(NULL, L"��ġ ���� ����", L"����", MB_OK);
		return false;
	}

	// 2D �ؽ�ó ���� (���� Ÿ�� ��).
	ID3D11Texture2D* pBackBufferTexture;
	hr = pSwapChain->GetBuffer(NULL, __uuidof(ID3D11Texture2D),
		(void**)&pBackBufferTexture);

	if (FAILED(hr))		 // E_FAIL
	{
		MessageBox(NULL, L"���� ���� ���� ����", L"����", MB_OK);
		return false;
	}

	// ���� Ÿ�� �� ����.
	hr = pDevice->CreateRenderTargetView(pBackBufferTexture, NULL,
		&pRenderTargetView);
	
	if (FAILED(hr))		 // E_FAIL
	{
		MessageBox(NULL, L"���� Ÿ�� �� ���� ����", L"����", MB_OK);
		return false;
	}
	
	// ����/���ٽ� ���� ����.
	if (!InitDepthStencilBuffer())
		return false;

	// ���� Ÿ�� �� ���ε�. ������ ���ο� ����.
	pDeviceContext->OMSetRenderTargets(1, &pRenderTargetView, depthStencilView);

	// ���ҽ� ����.
	Memory::SafeRelease(pBackBufferTexture);

	return true;
}

bool DXApp::ShaderCompile(Shape::Mesh* mesh)
{
	// ���� ���̴� ������/���ε�.
	if (!mesh->vertexShader.CompileShader()) return false;
	if (!mesh->vertexShader.BindShader(pDevice, pDeviceContext)) return false;

	// �ȼ� ���̴� ������/���ε�.
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
	// ����Ʈ ���� ����.
	D3D11_VIEWPORT viewport;
	ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = clientWidth;
	viewport.Height = clientHeight;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;

	// ����Ʈ ����.
	pDeviceContext->RSSetViewports(1, &viewport);
}

void DXApp::InitTransformMatrix(Shape::Mesh* mesh)
{
	// World x View x Projection;
	// ���� ���. (��ġ/ȸ��/������).
	world = XMMatrixIdentity();
	XMMATRIX rotationX = XMMatrixRotationX(XMConvertToRadians(mesh->rotation.x));
	XMMATRIX rotationY = XMMatrixRotationY(XMConvertToRadians(mesh->rotation.y));
	XMMATRIX translation = XMMatrixTranslation(mesh->position.x, mesh->position.y, mesh->position.z);

	world *= rotationX * rotationY * translation;

	// ī�޶� ��ġ.
	cameraPos = XMVectorSet(0.0f, 0.0f, -200.0f, 0.0f);

	// ī�޶� �ٶ󺸴� ��ġ.
	cameraTarget = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);

	// ī�޶� �� ����.
	cameraUp = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	// �� ���.
	view = XMMatrixLookAtLH(cameraPos, cameraTarget, cameraUp);

	// �þ߰�.
	float fovY = 75.0f * 3.14 / 180.0f;

	// ���� ���.
	projection = XMMatrixPerspectiveFovLH(fovY, (float)clientWidth / clientHeight, 1.0f, 1000.0f);

	// ��� ���� ������ ���� �� ���������� ���ε�.
	// ��ġ����� ���� ������ ä���.
	// DX�� ��켱 / HLSL�� ���켱.
	mesh->vertexShader.cbWVP.world = XMMatrixTranspose(world);
	mesh->vertexShader.cbWVP.view = XMMatrixTranspose(view);
	mesh->vertexShader.cbWVP.projection = XMMatrixTranspose(projection);

	// ���� ��ũ����.
	D3D11_BUFFER_DESC cbDesc;
	ZeroMemory(&cbDesc, sizeof(D3D11_BUFFER_DESC));
	cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbDesc.ByteWidth = sizeof(CBPerObject);
	cbDesc.CPUAccessFlags = 0;
	cbDesc.MiscFlags = 0;
	cbDesc.Usage = D3D11_USAGE_DEFAULT;

	// ���� ���ҽ� ����.
	D3D11_SUBRESOURCE_DATA cbData;
	ZeroMemory(&cbData, sizeof(D3D11_SUBRESOURCE_DATA));
	//cbData.pSysMem = &data;
	cbData.pSysMem = &mesh->vertexShader.cbWVP;

	// ��� ���� ����.
	HRESULT hr = pDevice->CreateBuffer(&cbDesc, &cbData, &cbBuffer);

	// ���������ο� ���ε�.
	pDeviceContext->VSSetConstantBuffers(0, 1, &cbBuffer);
}

bool DXApp::InitScene()
{
	// ����Ʈ ����.
	SetViewport();

	// �����Ͷ����� ���� ����.
	if (SetRasterizerState() == false)
		return false;

	// ���� ������Ʈ ����.
	//SetBlendState();

	return true;
}

bool DXApp::SetBlendState()
{
	// ���� �����̵� ������ ����.
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

	// ���� ������Ʈ ����.
	pDevice->CreateBlendState(&blendDesc, &alphaBlendState);

	// ���� ������Ʈ ���ε�.
	pDeviceContext->OMSetBlendState(alphaBlendState, 0, 0xffffffff);

	return true;
}

void DXApp::InitLightCB()
{
	// ��� ���۷� ������ ����Ʈ �� ����.
	CBLight cbLight;
	ZeroMemory(&cbLight, sizeof(cbLight));
	cbLight.lightPosition = XMVectorSet(500.0f, 500.0f, -500.0f, 1.0f);
	cbLight.cameraPosition = cameraPos;

	// ���ؽ� ���� ������(Descriptor).
	D3D11_BUFFER_DESC vbDesc;
	ZeroMemory(&vbDesc, sizeof(D3D11_BUFFER_DESC));
	vbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	vbDesc.ByteWidth = sizeof(CBLight);
	vbDesc.CPUAccessFlags = 0;
	vbDesc.MiscFlags = 0;
	vbDesc.Usage = D3D11_USAGE_DEFAULT;

	// ���� ���ҽ� ����.
	D3D11_SUBRESOURCE_DATA vbData;
	ZeroMemory(&vbData, sizeof(D3D11_SUBRESOURCE_DATA));
	vbData.pSysMem = &cbLight;

	// ���� ����.
	ID3D11Buffer* lightBuffer;
	pDevice->CreateBuffer(&vbDesc, &vbData, &lightBuffer);

	// ���� ���������ο� ���ε�.
	pDeviceContext->VSSetConstantBuffers(1, 1, &lightBuffer);
}

bool DXApp::InitDepthStencilBuffer()
{
	// ���� ������ ����.
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

	// ����/���ٽ� ���� ����.
	HRESULT hr;
	hr = pDevice->CreateTexture2D(&depthStencilDesc, NULL, &depthStencilBuffer);
	if (FAILED(hr))
	{
		MessageBox(NULL, L"����/���ٽ� ���� ���� ����", L"����", MB_OK);
		return false;
	}

	// ����/���ٽ� �� ����.
	hr = pDevice->CreateDepthStencilView(depthStencilBuffer, NULL, &depthStencilView);
	if (FAILED(hr))
	{
		MessageBox(NULL, L"����/���ٽ� �� ���� ����", L"����", MB_OK);
		return false;
	}

	return true;
}

bool DXApp::InitDirectInput(HINSTANCE hinstance)
{
	// �Է� �ʱ�ȭ.
	HRESULT hr = DirectInput8Create(hinstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&directInput, NULL);
	if (FAILED(hr))
	{
		MessageBox(NULL, L"���̷�Ʈ �Է� �ʱ�ȭ ����", L"����", MB_OK);
		return false;
	}

	// ��ġ �ʱ�ȭ.
	hr = directInput->CreateDevice(GUID_SysKeyboard, &diKeyboard, NULL);
	if (FAILED(hr))
	{
		MessageBox(NULL, L"Ű���� ��ġ �ʱ�ȭ ����", L"����", MB_OK);
		return false;
	}

	hr = directInput->CreateDevice(GUID_SysMouse, &diMouse, NULL);
	if (FAILED(hr))
	{
		MessageBox(NULL, L"���콺 ��ġ �ʱ�ȭ ����", L"����", MB_OK);
		return false;
	}

	// ������ ���� ����.
	hr = diKeyboard->SetDataFormat(&c_dfDIKeyboard);
	if (FAILED(hr))
	{
		MessageBox(NULL, L"Ű���� ���� ���� ����", L"����", MB_OK);
		return false;
	}

	// ���� ���� ����.
	hr = diKeyboard->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	if (FAILED(hr))
	{
		MessageBox(NULL, L"Ű���� ���� ���� ���� ����", L"����", MB_OK);
		return false;
	}

	hr = diMouse->SetDataFormat(&c_dfDIMouse);
	if (FAILED(hr))
	{
		MessageBox(NULL, L"���콺 ������ ���� ����", L"����", MB_OK);
		return false;
	}

	hr = diMouse->SetCooperativeLevel(hwnd, DISCL_EXCLUSIVE | DISCL_FOREGROUND | DISCL_NOWINKEY);
	if (FAILED(hr))
	{
		MessageBox(NULL, L"���콺 ���� ���� ���� ����", L"����", MB_OK);
		return false;
	}

	return true;
}

void DXApp::DetectInput(double deltaTime)
{
	// Ű���� / ���콺 �Է� ������ ���� �� ����.
	DIMOUSESTATE currentMouseState;
	BYTE keyboardState[256];

	// �Է� ȹ��.
	diKeyboard->Acquire();
	diMouse->Acquire();

	// Ű����/���콺 ��ġ ���� ������.
	diKeyboard->GetDeviceState(sizeof(keyboardState), (LPVOID)&keyboardState);
	diMouse->GetDeviceState(sizeof(currentMouseState), &currentMouseState);

	// Ű �Է� Ȯ��.
	if (keyboardState[DIK_ESCAPE] & 0x80)
		PostMessage(hwnd, WM_DESTROY, 0, 0);

	if (keyboardState[DIK_LEFT] & 0x80) moveLeftRight -= 1.0f * deltaTime;
	if (keyboardState[DIK_RIGHT] & 0x80) moveLeftRight += 1.0f * deltaTime;

	if (keyboardState[DIK_UP] & 0x80) moveBackForward += 1.0f * deltaTime;
	if (keyboardState[DIK_DOWN] & 0x80) moveBackForward -= 1.0f * deltaTime;

	// ���콺 �Է�.
	if (currentMouseState.lX != mouseLastState.lX ||
		currentMouseState.lY != mouseLastState.lY)
	{
		cameraYaw += currentMouseState.lX * 0.001f;
		cameraPitch += currentMouseState.lY * 0.001f;
	}

	// �̹� �������� ���콺 �Է��� ����.
	mouseLastState = currentMouseState;

	//// ī�޶� ����.
	//UpdateCamera();
}

void DXApp::UpdateCamera()
{
	// ī�޶� ȸ��.
	cameraRotationMatrix = XMMatrixRotationRollPitchYaw(cameraPitch, cameraYaw, 0.0f);

	// ī�޶� ���� ���� ����.
	cameraTarget = XMVector3TransformCoord(defaultForward, cameraRotationMatrix);
	cameraTarget = XMVector3Normalize(cameraTarget);

	XMMATRIX rotateYTemp;
	rotateYTemp = XMMatrixRotationY(cameraYaw);

	// ī�޶� ������ ���� ���� (ȸ�� �� �����ؼ� ���� ����).
	cameraRight = XMVector3TransformCoord(defaultRight, rotateYTemp);
	cameraUp = XMVector3TransformCoord(cameraUp, rotateYTemp);
	cameraForward = XMVector3TransformCoord(defaultForward, rotateYTemp);

	// ī�޶� ��ġ �̵�.
	cameraPos += moveLeftRight * cameraRight;
	cameraPos += moveBackForward * cameraForward;

	// ����� �Է� �ʱ�ȭ.
	moveLeftRight = 0.0f;
	moveBackForward = 0.0f;

	// ī�޶� �ٶ󺸴� ���� ���Ϳ� ��ġ ����.
	cameraTarget = cameraPos + cameraTarget;

	// �� ��� ����.
	view = XMMatrixLookAtLH(cameraPos, cameraTarget, cameraUp);
}

bool DXApp::SetRasterizerState()
{
	// �����Ͷ����� ������ ����.
	D3D11_RASTERIZER_DESC rsDesc;
	ZeroMemory(&rsDesc, sizeof(D3D11_RASTERIZER_DESC));
	rsDesc.CullMode = D3D11_CULL_BACK;		// �޸� �ȱ׸���.
	rsDesc.FillMode = D3D11_FILL_SOLID;	// ������ ���������� �׸���.

	HRESULT hr;
	// �����Ͷ����� ���� ����.
	hr = pDevice->CreateRasterizerState(&rsDesc, &rsState);
	if (FAILED(hr))
		return false;

	// ���ε�.
	pDeviceContext->RSSetState(rsState);

	return true;
}

LRESULT DXApp::MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_KEYDOWN:
		// ESC Ű ���ȴ��� Ȯ��.
		if (wParam == VK_ESCAPE)
		{
			// â �ݱ� ���θ� ���� �޽��� �ڽ� ����.
			if (MessageBox(NULL, L"�����Ͻðڽ��ϱ�?", L"����", MB_YESNO | MB_ICONQUESTION) == IDYES)
			{
				// Yes(��) Ű ������ â ����.
				DestroyWindow(this->hwnd);
			}
		}
		return 0;

		// â�� �����Ǹ� ����.
	case WM_DESTROY:
		// ���α׷� ���� �޽��� ȣ��.
		PostQuitMessage(0);
		return 0;

	default: return DefWindowProc(hwnd, msg, wParam, lParam);
	}
}