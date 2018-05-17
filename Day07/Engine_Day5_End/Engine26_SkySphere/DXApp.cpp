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
	appTitle = L"Engine25 - Toon Shader";
	wndStyle = WS_OVERLAPPEDWINDOW;
	g_pApp = this;

	// DX ���� ��� ���� �ʱ�ȭ.
	pDevice = NULL;
	pDeviceContext = NULL;
	pSwapChain = NULL;
	pRenderTargetView = NULL;

	vertexInputLayout = NULL;
}

DXApp::~DXApp()
{
	// Ǯ ��ũ�� ��� ����.
	pSwapChain->SetFullscreenState(false, NULL);

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

	Memory::SafeRelease(pSkymapResourceView);
	Memory::SafeRelease(pDSLessEqual);
	Memory::SafeRelease(pRSCullNone);
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
	if (!InitWindow()) return false;

	// DirectX �ʱ�ȭ.
	if (!InitDirect3D()) return false;

	// ��� �ʱ�ȭ.
	if (!InitScene()) return false;

	// ���̷�Ʈ �Է� �ʱ�ȭ.
	if (!InitDirectInput(hinstance)) return false;

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
	if (hwnd == NULL) return false;

	// â ����.
	ShowWindow(hwnd, SW_SHOW);

	// â ������Ʈ�ϱ�.
	UpdateWindow(hwnd);

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
	if (!mesh->vertexShader.CreateShader(pDevice)) return false;

	// �ȼ� ���̴� ������/���ε�.
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
	// WVP ��� �ʱ�ȭ.
	InitWorldMatrix(mesh);
	InitViewMatrix(mesh);
	InitProjectionMatrix(mesh);

	// ��� ���� ������ ���� �� ���������� ���ε�.
	// ��ġ����� ���� ������ ä���.
	// DX�� ��켱 / HLSL�� ���켱.
	mesh->vertexShader.cbWVP.world = XMMatrixTranspose(world);
	mesh->vertexShader.cbWVP.view = XMMatrixTranspose(view);
	mesh->vertexShader.cbWVP.projection = XMMatrixTranspose(projection);

	// ������� ����.
	InitWVPBuffer(mesh);

	// ���������ο� ���ε�.
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
	// ī�޶� ��ġ.
	cameraPos = XMVectorSet(0.0f, 0.0f, -200.0f, 0.0f);

	// ī�޶� �ٶ󺸴� ��ġ.
	cameraTarget = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);

	// ī�޶� �� ����.
	cameraUp = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	// �� ���.
	view = XMMatrixLookAtLH(cameraPos, cameraTarget, cameraUp);
}

void DXApp::UpdateViewMatrix(Shape::Mesh * mesh)
{
	mesh->vertexShader.cbWVP.view = XMMatrixTranspose(view);
	pDeviceContext->UpdateSubresource(cbBuffer, 0, 0, &mesh->vertexShader.cbWVP, 0, 0);
}

void DXApp::InitProjectionMatrix(Shape::Mesh * mesh)
{
	// �þ߰�.
	float fovY = 75.0f * 3.14 / 180.0f;

	// ���� ���.
	projection = XMMatrixPerspectiveFovLH(fovY, (float)clientWidth / (float)clientHeight, 1.0f, 100.0f);
}

bool DXApp::InitWVPBuffer(Shape::Mesh * mesh)
{
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
	cbData.pSysMem = &mesh->vertexShader.cbWVP;

	// ��� ���� ����.
	if (FAILED(pDevice->CreateBuffer(&cbDesc, &cbData, &cbBuffer)))
	{
		OutputDebugString(L"WVP ������� ���� ����");
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
	
	// ���������ο� ���ε�.
	pDeviceContext->VSSetConstantBuffers(0, 1, &cbBuffer);
}

bool DXApp::InitScene()
{
	// �޽� ���� ����.
	InitMeshInfo();

	// -------------------- New -------------------- //
	if (!ShaderCompile(&sphereMesh)) return false;
	if (!InitVertexBuffer(&sphereMesh)) return false;
	if (!InitIndexBuffer(&sphereMesh)) return false;
	if (!InitInputLayout(&sphereMesh)) return false;
	pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// �ؽ�ó �ε�.
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
		// ���̴� ������/������Ʈ ����.
		if (!ShaderCompile(&meshes[ix])) return false;

		// ���� ���� ����.
		if (!InitVertexBuffer(&meshes[ix])) return false;

		// �ε��� ���� ����.
		if (!InitIndexBuffer(&meshes[ix])) return false;

		// �Է� ���̾ƿ� ����.
		if (!InitInputLayout(&meshes[ix])) return false;

		// �⺻ �������� ���� (TRIANGLELIST�� ����).
		pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		// �ؽ�ó �ε�.
		if (!meshes[ix].pixelShader.LoadTextures(pDevice)) return false;

		// ���÷� ������Ʈ ����.
		if (!meshes[ix].pixelShader.CreateSamplerState(pDevice)) return false;

		// ���� ��ȯ.
		InitTransformMatrix(&meshes[ix]);

		// ����Ʈ ��� ���� ����.
		InitLightCB();

		// ����Ʈ ��� ���� ���ε�.
		BindLightCB();
	}

	// ����Ʈ ����.
	SetViewport();

	// ���� ������Ʈ ����.
	//SetBlendState();

	// �����Ͷ����� ���� ����.
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
	// ���� �����̵� ������ ����.
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

	// ���� ������Ʈ ����.
	pDevice->CreateBlendState(&blendDesc, &alphaBlendState);

	// ���� ������Ʈ ���ε�.
	pDeviceContext->OMSetBlendState(alphaBlendState, 0, 0xffffffff);

	return true;
}

void DXApp::InitLightCB()
{
	// ��� ���۷� ������ ����Ʈ �� ����.
	cbLight;
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
	pDevice->CreateBuffer(&vbDesc, &vbData, &lightBuffer);
}

void DXApp::UpdateLightCB()
{
	pDeviceContext->UpdateSubresource(lightBuffer, 0, 0, &cbLight, 0, 0);
	pDeviceContext->VSSetConstantBuffers(1, 1, &lightBuffer);
}

void DXApp::BindLightCB()
{
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

	if (keyboardState[DIK_LEFT] & 0x80 || keyboardState[DIK_A] & 0x80) moveLeftRight -= 1.0f * cameraMoveSpeed * deltaTime;
	if (keyboardState[DIK_RIGHT] & 0x80 || keyboardState[DIK_D] & 0x80) moveLeftRight += 1.0f * cameraMoveSpeed * deltaTime;

	if (keyboardState[DIK_UP] & 0x80 || keyboardState[DIK_W] & 0x80) moveBackForward += 1.0f  * cameraMoveSpeed * deltaTime;
	if (keyboardState[DIK_DOWN] & 0x80 || keyboardState[DIK_S] & 0x80) moveBackForward -= 1.0f  * cameraMoveSpeed * deltaTime;

	// ���콺 �Է�.
	if (currentMouseState.lX != mouseLastState.lX ||
		currentMouseState.lY != mouseLastState.lY)
	{
		cameraYaw += currentMouseState.lX * 0.001f;
		cameraPitch += currentMouseState.lY * 0.001f;
	}

	// �̹� �������� ���콺 �Է��� ����.
	mouseLastState = currentMouseState;
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
	//D3D11_RASTERIZER_DESC rsDesc;
	ZeroMemory(&rsDesc, sizeof(D3D11_RASTERIZER_DESC));
	rsDesc.CullMode = D3D11_CULL_NONE;		// �޸� �ȱ׸���.
	rsDesc.FillMode = D3D11_FILL_SOLID;	// ������ ���������� �׸���.

	HRESULT hr;
	// �����Ͷ����� ���� ����.
	hr = pDevice->CreateRasterizerState(&rsDesc, &rsState);
	if (FAILED(hr))
	{
		OutputDebugString(L"�����Ͷ����� ���� ���� ����");
		OutputDebugString(L"\n");
		return false;
	}

	// ���ε�.
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
	// ��ü �޽�.
	sphereMesh;
	sphereMesh.fbxName = fbxNameSphere;
	sphereMesh.vertexShader = ShaderUtil::VertexShader(L"Shaders//SkyBoxVS.fx", "main", "vs_5_0");
	sphereMesh.pixelShader = ShaderUtil::PixelShader(L"Shaders//SkyBoxPS.fx", "main", "ps_5_0");
	sphereMesh.pixelShader.pTextures.push_back(cubeMap);
	sphereMesh.position = XMFLOAT3(0.0f, 0.0f, 100.0f);
	sphereMesh.rotation = XMFLOAT3(0.0f, 0.0f, 0.0f);
	sphereMesh.scale = XMFLOAT3(1.0f, 1.0f, 1.0f);
	// ---------------------- New ---------------------- //

	// �ٴ� �޽�.
	Shape::Mesh planeMesh;
	planeMesh.fbxName = fbxNamePlane;
	planeMesh.vertexShader = ShaderUtil::VertexShader(L"Shaders//LabertianDiffuseVS.fx", "main", "vs_5_0");
	planeMesh.pixelShader = ShaderUtil::PixelShader(L"Shaders//LabertianDiffusePS.fx", "main", "ps_5_0");
	planeMesh.pixelShader.pTextures.push_back(grassTexture);
	planeMesh.position = XMFLOAT3(0.0f, -120.0f, -30.0f);
	planeMesh.rotation = XMFLOAT3(-90.0f, 0.0f, 0.0f);
	planeMesh.scale = XMFLOAT3(1000.0f, 1000.0f, 1.0f);
	meshes.push_back(planeMesh);

	// �޽� (��ǻ��� ����).
	Shape::Mesh diffuseMesh;
	diffuseMesh.fbxName = fbxNameTPP;
	diffuseMesh.vertexShader = ShaderUtil::VertexShader(L"Shaders//LabertianDiffuseVS.fx", "main", "vs_5_0");
	diffuseMesh.pixelShader = ShaderUtil::PixelShader(L"Shaders//LabertianDiffusePS.fx", "main", "ps_5_0");
	diffuseMesh.pixelShader.pTextures.push_back(diffuseMap);
	diffuseMesh.position = XMFLOAT3(-450.0f, -120.0f, -30.0f);
	diffuseMesh.rotation = XMFLOAT3(-90.0f, 180.0f, 0.0f);
	meshes.push_back(diffuseMesh);

	// �޽� (��ǻ��� ����).
	Shape::Mesh halfLambertMesh;
	halfLambertMesh.fbxName = fbxNameTPP;
	halfLambertMesh.vertexShader = ShaderUtil::VertexShader(L"Shaders//HalfLabertianDiffuseVS.fx", "main", "vs_5_0");
	halfLambertMesh.pixelShader = ShaderUtil::PixelShader(L"Shaders//HalfLabertianDiffusePS.fx", "main", "ps_5_0");
	halfLambertMesh.pixelShader.pTextures.push_back(diffuseMap);
	halfLambertMesh.position = XMFLOAT3(-300.0f, -120.0f, -30.0f);
	halfLambertMesh.rotation = XMFLOAT3(-90.0f, 180.0f, 0.0f);
	meshes.push_back(halfLambertMesh);

	// �޽� (��ǻ��� + �� ���̵� ����).
	Shape::Mesh toonMesh;
	toonMesh.fbxName = fbxNameTPP;
	toonMesh.vertexShader = ShaderUtil::VertexShader(L"Shaders//ToonVS.fx", "main", "vs_5_0");
	toonMesh.pixelShader = ShaderUtil::PixelShader(L"Shaders//ToonPS.fx", "main", "ps_5_0");
	toonMesh.pixelShader.pTextures.push_back(diffuseMap);
	toonMesh.position = XMFLOAT3(-150.0f, -120.0f, -30.0f);
	toonMesh.rotation = XMFLOAT3(-90.0f, 180.0f, 0.0f);
	meshes.push_back(toonMesh);

	// �޽� (��ǻ��� + ����ŧ�� + ��ָ� ����).
	Shape::Mesh normalMesh;
	normalMesh.fbxName = fbxNameTPP;
	normalMesh.vertexShader = ShaderUtil::VertexShader(L"Shaders//NormalVS.fx", "main", "vs_5_0");
	normalMesh.pixelShader = ShaderUtil::PixelShader(L"Shaders//NormalPS.fx", "main", "ps_5_0");
	normalMesh.pixelShader.pTextures.push_back(diffuseMap);
	normalMesh.pixelShader.pTextures.push_back(normalMap);
	normalMesh.position = XMFLOAT3(0.0f, -120.0f, -30.0f);
	normalMesh.rotation = XMFLOAT3(-90.0f, 180.0f, 0.0f);

	meshes.push_back(normalMesh);

	// �޽� (��ǻ��� + ����ŧ�� + ��ָ� + ť��� ����).
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