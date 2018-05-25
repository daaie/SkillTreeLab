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
	appTitle = L"Engine16 - Direct Input";
	wndStyle = WS_OVERLAPPEDWINDOW;
	g_pApp = this;

	// DX ���� ��� ���� �ʱ�ȭ.
	pDevice = NULL;
	pDeviceContext = NULL;
	pSwapChain = NULL;
	pRenderTargetView = NULL;

	vertexBuffer = NULL;
	vertexShader = NULL;
	pixelShader = NULL;
	vertexShaderBuffer = NULL;
	pixelShaderBuffer = NULL;
	vertexInputLayout = NULL;

	indexBuffer = NULL;
}

DXApp::~DXApp()
{
	// �޸� ����.
	Memory::SafeRelease(pDevice);
	Memory::SafeRelease(pDeviceContext);
	Memory::SafeRelease(pSwapChain);
	Memory::SafeRelease(pRenderTargetView);

	Memory::SafeRelease(vertexBuffer);
	Memory::SafeRelease(vertexShader);
	Memory::SafeRelease(pixelShader);
	Memory::SafeRelease(vertexShaderBuffer);
	Memory::SafeRelease(pixelShaderBuffer);
	Memory::SafeRelease(vertexInputLayout);

	Memory::SafeRelease(indexBuffer);

	Memory::SafeRelease(cbBuffer);

	Memory::SafeRelease(pTexture);
	Memory::SafeRelease(pTextureSamplerState);
	
	Memory::SafeRelease(rsState);

	// ���� ������Ʈ ����.
	Memory::SafeRelease(alphaBlendState);

	// ����/���ٽ� ���� ���� ���ҽ� ����.
	Memory::SafeRelease(depthStencilView);
	Memory::SafeRelease(depthStencilBuffer);
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

bool DXApp::ShaderCompile()
{
	// ���̴� ������.
	HRESULT hr;
	// ���� ���̴� ������.
	hr = D3DX11CompileFromFile(L"VertexShader.fx", NULL, NULL, "main", "vs_5_0", NULL, NULL, NULL, &vertexShaderBuffer, NULL, NULL);

	if (FAILED(hr))
	{
		MessageBox(NULL, L"���� ���̴� ������ ����", L"����", MB_OK);
		return false;
	}

	// ���� ���̴� ������Ʈ ����.
	hr = pDevice->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(),
		vertexShaderBuffer->GetBufferSize(), NULL, &vertexShader);
	if (FAILED(hr))
	{
		MessageBox(NULL, L"���� ���̴� ������Ʈ ���� ����", L"����", MB_OK);
		return false;
	}

	// ���� ���̴� ����.(���ε�).
	pDeviceContext->VSSetShader(vertexShader, NULL, NULL);

	// �ȼ� ���̴�.
	hr = D3DX11CompileFromFile(L"PixelShader.fx", NULL, NULL, "main",
		"ps_5_0", NULL, NULL, NULL, &pixelShaderBuffer, NULL, NULL);
	if (FAILED(hr))
	{
		MessageBox(NULL, L"�ȼ� ���̴� ������ ����", L"����", MB_OK);
		return false;
	}

	// �ȼ� ���̴� ������Ʈ ����.
	hr = pDevice->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(),
		pixelShaderBuffer->GetBufferSize(), NULL, &pixelShader);
	if (FAILED(hr))
	{
		MessageBox(NULL, L"�ȼ� ���̴� ������Ʈ ���� ����", L"����", MB_OK);
		return false;
	}

	// �ȼ� ���̴� ���ε�.
	pDeviceContext->PSSetShader(pixelShader, NULL, NULL);

	return true;
}

void DXApp::InitVertexBuffer()
{
	// FBX �ε�.
	HRESULT hr = LoadFBX("sphere.fbx", &pVertices);
	if (FAILED(hr))
		return;

	// ���� ���� ����.
	nVertices = pVertices.size();

	// ���ؽ� ���� ������(Descriptor).
	D3D11_BUFFER_DESC vbDesc;
	ZeroMemory(&vbDesc, sizeof(D3D11_BUFFER_DESC));
	vbDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbDesc.ByteWidth = sizeof(Vertex) * nVertices;
	vbDesc.CPUAccessFlags = 0;
	vbDesc.MiscFlags = 0;
	vbDesc.Usage = D3D11_USAGE_DEFAULT;

	// ���� ���ҽ� ����.
	D3D11_SUBRESOURCE_DATA vbData;
	ZeroMemory(&vbData, sizeof(D3D11_SUBRESOURCE_DATA));
	vbData.pSysMem = &pVertices[0];

	// ���� ���� ����.
	pDevice->CreateBuffer(&vbDesc, &vbData, &vertexBuffer);

	UINT stride = sizeof(Vertex);
	UINT offset = 0;

	// ���� ���� ���ε�.
	pDeviceContext->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
}

void DXApp::InitIndexBuffer()
{
	// �ε��� �迭 �����.
	DWORD indices[] =
	{
		// Front Face
		0,  1,  2,
		0,  2,  3,

		// Back Face
		4,  5,  6,
		4,  6,  7,

		// Top Face
		8,  9, 10,
		8, 10, 11,

		// Bottom Face
		12, 13, 14,
		12, 14, 15,

		// Left Face
		16, 17, 18,
		16, 18, 19,

		// Right Face
		20, 21, 22,
		20, 22, 23
	};

	// �ε��� ���� ����.
	nIndices = ARRAYSIZE(indices);

	// �ε��� ���� ������(Descriptor).
	D3D11_BUFFER_DESC ibDesc;
	ZeroMemory(&ibDesc, sizeof(D3D11_BUFFER_DESC));
	ibDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibDesc.ByteWidth = sizeof(DWORD) * ARRAYSIZE(indices);
	ibDesc.CPUAccessFlags = 0;
	ibDesc.MiscFlags = 0;
	ibDesc.Usage = D3D11_USAGE_DEFAULT;

	// ���� ���ҽ� ����.
	D3D11_SUBRESOURCE_DATA ibData;
	ZeroMemory(&ibData, sizeof(D3D11_SUBRESOURCE_DATA));
	ibData.pSysMem = indices;

	// �ε��� ���� ����.
	pDevice->CreateBuffer(&ibDesc, &ibData, &indexBuffer);

	// �ε��� ���� ���ε�.
	pDeviceContext->IASetIndexBuffer(indexBuffer,
		DXGI_FORMAT_R32_UINT, 0);
}

bool DXApp::InitInputLayout()
{
	// �Է� ���̾ƿ�.
	// D3D11_APPEND_ALIGNED_ELEMENT
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	// �Է� ���̾ƿ� ����.
	HRESULT hr = pDevice->CreateInputLayout(layout, ARRAYSIZE(layout),
		vertexShaderBuffer->GetBufferPointer(),
		vertexShaderBuffer->GetBufferSize(), &vertexInputLayout);

	if (FAILED(hr))
	{
		MessageBox(NULL, L"�Է� ���̾ƿ� ���� ����", L"����", MB_OK);
		return false;
	}

	// �Է� ���̾ƿ� ���ε�.
	pDeviceContext->IASetInputLayout(vertexInputLayout);
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

// ���� ��ȯ ��� ����.
void DXApp::InitTransformMatrix()
{
	// World x View x Projection;
	// ���� ���. (��ġ/ȸ��/������).
	world = XMMatrixIdentity();
	float rotX = XMConvertToRadians(-90.0f);
	//float rotY = XMConvertToRadians(180.0f);
	XMMATRIX rotationX = XMMatrixRotationX(rotX);
	//XMMATRIX rotationY = XMMatrixRotationY(rotY);

	//world *= rotationX * rotationY;
	world *= rotationX;

	// ī�޶� ��ġ.
	cameraPos = XMVectorSet(0.0f, 3.0f, -3.0f, 0.0f);

	// ī�޶� �ٶ󺸴� ��ġ.
	cameraTarget = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);

	// ī�޶� �� ����.
	cameraUp = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	// �� ���.
	view = XMMatrixLookAtLH(cameraPos, cameraTarget, cameraUp);

	// �þ߰�.
	float fovY = 75.0f * 3.14 / 180.0f;

	// ���� ���.
	projection = XMMatrixPerspectiveFovLH(fovY, (float)clientWidth / clientHeight,
		1.0f, 1000.0f);

	// ��� ���� ������ ���� �� ���������� ���ε�.
	// ��ġ����� ���� ������ ä���.
	// DX�� ��켱 / HLSL�� ���켱.
	CBPerObject data;
	data.world = XMMatrixTranspose(world);
	data.view = XMMatrixTranspose(view);
	data.projection = XMMatrixTranspose(projection);

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
	cbData.pSysMem = &data;

	// ��� ���� ����.
	HRESULT hr = pDevice->CreateBuffer(&cbDesc, &cbData, &cbBuffer);

	// ���������ο� ���ε�.
	pDeviceContext->VSSetConstantBuffers(0, 1, &cbBuffer);
}

bool DXApp::InitScene()
{
	//// ����/���ٽ� ���� ����.
	//if (!InitDepthStencilBuffer())
	//	return false;

	// ���̴� ������.
	if (!ShaderCompile())
		return false;

	// ���� ���� ����.
	InitVertexBuffer();

	// �ε��� ���� ����.
	//InitIndexBuffer();

	// �Է� ���̾ƿ� ����.
	if (!InitInputLayout())
		return false;

	// �⺻ �������� ���� (TRIANGLELIST�� ����).
	pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// ����Ʈ ����.
	SetViewport();

	// ���� ��ȯ ��� ����.
	InitTransformMatrix();

	// ����Ʈ ���� ���� �� ���ε�.
	InitLightCB();

	// �ؽ�ó �ε�(���÷� �ʱ�ȭ).
	//if (!InitTexture()) return false;

	// �ؽ�ó ���ε�.
	//pDeviceContext->PSSetShaderResources(0, 1, &pTexture);
	// ���÷� ������Ʈ ���ε�.
	//pDeviceContext->PSSetSamplers(0, 1, &pTextureSamplerState);

	// �����Ͷ����� ���� ����.
	if (SetRasterizerState() == false)
		return false;

	// ���� ������Ʈ ����.
	//SetBlendState();

	return true;
}

bool DXApp::InitTexture()
{
	HRESULT hr;
	// �ؽ�ó ���� �ε�.
	hr = D3DX11CreateShaderResourceViewFromFile(pDevice,
		L"unreal.png", NULL, NULL, &pTexture, NULL);

	if (FAILED(hr))
		return false;

	// ���÷� ���� ������ ����.
	D3D11_SAMPLER_DESC samplerDesc;
	ZeroMemory(&samplerDesc, sizeof(D3D11_SAMPLER_DESC));
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	// ���÷� ����(State) ����.
	hr = pDevice->CreateSamplerState(&samplerDesc, &pTextureSamplerState);

	if (FAILED(hr))
		return false;

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

HRESULT DXApp::LoadFBX(const char * fileName, std::vector<Vertex>* pOutVertex)
{
	// fbxManager�� �������� ������ ����.
	if (fbxManager == NULL)
	{
		// �ʱ� SDK Manager ����.
		fbxManager = FbxManager::Create();

		FbxIOSettings* pIOSettings = FbxIOSettings::Create(fbxManager, IOSROOT);
		fbxManager->SetIOSettings(pIOSettings);
	}

	FbxImporter* pImporter = FbxImporter::Create(fbxManager, "");
	FbxScene* pFbxScene = FbxScene::Create(fbxManager, "");

	// ������ �ʱ�ȭ.
	if (!pImporter->Initialize(fileName, -1, fbxManager->GetIOSettings()))
	{
		return E_FAIL;
	}

	// 3D �� ����Ʈ.
	if (!pImporter->Import(pFbxScene))
	{
		return E_FAIL;
	}

	// ������ ����.
	pImporter->Destroy();

	// FbxNode �б�.
	FbxNode* pFbxRootNode = pFbxScene->GetRootNode();
	if (pFbxRootNode == NULL) return E_FAIL;

	int childCount = pFbxRootNode->GetChildCount();
	for (int ix = 0; ix < childCount; ++ix)
	{
		// �ڽ� ���� ��� �о����.
		FbxNode* pFbxChildNode = pFbxRootNode->GetChild(ix);
		if (pFbxChildNode->GetNodeAttribute() == NULL)
			continue;

		// ��� �Ӽ� Ÿ�� Ȯ��.
		FbxNodeAttribute::EType attrType 
			= pFbxChildNode->GetNodeAttribute()->GetAttributeType();

		// ��� Ÿ���� �޽��� �ƴ� ��쿡�� ����.
		if (attrType != FbxNodeAttribute::EType::eMesh)
			continue;

		// �޽� ���� �б�.
		FbxMesh* pMesh = pFbxChildNode->GetMesh();

		
		// ��Ʈ�� ����Ʈ �� �迭 ���� ������.
		FbxVector4* pVertices = pMesh->GetControlPoints();
		// ������ �� Ȯ��.
		int polygonCount = pMesh->GetPolygonCount();
		int vertexCounter = 0;
		for (int jx = 0; jx < polygonCount; ++jx)
		{
			// �������� �����ϴ� ���� ���� ���ϱ�.
			int vertexCount = pMesh->GetPolygonSize(jx);
			for (int kx = 0; kx < vertexCount; ++kx)
			{
				// �ﰢ�� ��ȣ, ���� ��ȣ�� ���� ���� �б�.
				int vertexIndex = pMesh->GetPolygonVertex(jx, kx);

				// ���� ��ġ ���� �о����.
				Vertex vertex;
				vertex.position.x = (float)pVertices[vertexIndex].mData[0];
				vertex.position.y = (float)pVertices[vertexIndex].mData[1];
				vertex.position.z = (float)pVertices[vertexIndex].mData[2];

				// ��� �� �о����.
				XMFLOAT3 vertexNormal = ReadNormal(pMesh, vertexIndex, vertexCounter);

				// ���� ����ü�� ��� �� �߰�.
				vertex.normal = vertexNormal;

				// ���� �迭�� �߰�.
				pOutVertex->push_back(vertex);
				++vertexCounter;
			}
		}
	}

	return S_OK;
}

XMFLOAT3 DXApp::ReadNormal(FbxMesh * pMesh, int controlPointIndex, int vertexCounter)
{
	XMFLOAT3 normal(0.0f, 0.0f, 0.0f);
	if (pMesh->GetElementNormalCount() < 1)
	{
		MessageBox(NULL, L"��� ���� ��ȿ���� �ʽ��ϴ�.", L"����", MB_OK);
		return normal;
	}

	FbxGeometryElementNormal* vertexNormal = pMesh->GetElementNormal(0);
	switch (vertexNormal->GetMappingMode())
	{
	case FbxGeometryElement::eByControlPoint:
		switch (vertexNormal->GetReferenceMode())
		{
		case FbxGeometryElement::eDirect:
		{
			normal.x = (float)vertexNormal->GetDirectArray().GetAt(controlPointIndex).mData[0];
			normal.y = (float)vertexNormal->GetDirectArray().GetAt(controlPointIndex).mData[1];
			normal.z = (float)vertexNormal->GetDirectArray().GetAt(controlPointIndex).mData[2];
		}
		break;
		case FbxGeometryElement::eIndexToDirect:
		{
			int index = vertexNormal->GetIndexArray().GetAt(controlPointIndex);
			normal.x = (float)vertexNormal->GetDirectArray().GetAt(index).mData[0];
			normal.y = (float)vertexNormal->GetDirectArray().GetAt(index).mData[1];
			normal.z = (float)vertexNormal->GetDirectArray().GetAt(index).mData[2];
		}
		break;

		default: return normal;
		}
		break;

	case FbxGeometryElement::eByPolygonVertex:
	{
		switch (vertexNormal->GetReferenceMode())
		{
		case FbxGeometryElement::eDirect:
		{
			normal.x = (float)vertexNormal->GetDirectArray().GetAt(vertexCounter).mData[0];
			normal.y = (float)vertexNormal->GetDirectArray().GetAt(vertexCounter).mData[1];
			normal.z = (float)vertexNormal->GetDirectArray().GetAt(vertexCounter).mData[2];
		}
		break;

		case FbxGeometryElement::eIndexToDirect:
		{
			int index = vertexNormal->GetIndexArray().GetAt(vertexCounter);
			normal.x = (float)vertexNormal->GetDirectArray().GetAt(index).mData[0];
			normal.y = (float)vertexNormal->GetDirectArray().GetAt(index).mData[1];
			normal.z = (float)vertexNormal->GetDirectArray().GetAt(index).mData[2];
		}
		break; default: return normal;
		}
	}
	break;

	default: return normal;
	}

	return normal;
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