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
	clientWidth = 1600;
	clientHeight = 1200;
	appTitle = L"Engine09 - Render State";
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
			// Update Scene.
			Update();

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

	// ���� Ÿ�� �� ���ε�. ������ ���ο� ����.
	pDeviceContext->OMSetRenderTargets(1, &pRenderTargetView, NULL);

	// ���ҽ� ����.
	Memory::SafeRelease(pBackBufferTexture);

	return true;
}

bool DXApp::ShaderCompile()
{
	// ���̴� ������.
	HRESULT hr;
	// ���� ���̴� ������.
	hr = D3DX11CompileFromFile(L"Effects.fx", NULL, NULL, "VS_Main", "vs_5_0", NULL, NULL, NULL, &vertexShaderBuffer, NULL, NULL);

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
	hr = D3DX11CompileFromFile(L"Effects.fx", NULL, NULL, "PS_Main",
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
	// ���� �����.
	Vertex vertices[] =
	{
		// Front Face
		Vertex(-1.0f, -1.0f, -1.0f, 0.0f, 1.0f),
		Vertex(-1.0f,  1.0f, -1.0f, 0.0f, 0.0f),
		Vertex(1.0f,  1.0f, -1.0f, 1.0f, 0.0f),
		Vertex(1.0f, -1.0f, -1.0f, 1.0f, 1.0f),

		// Back Face
		Vertex(-1.0f, -1.0f, 1.0f, 1.0f, 1.0f),
		Vertex(1.0f, -1.0f, 1.0f, 0.0f, 1.0f),
		Vertex(1.0f,  1.0f, 1.0f, 0.0f, 0.0f),
		Vertex(-1.0f,  1.0f, 1.0f, 1.0f, 0.0f),

		// Top Face
		Vertex(-1.0f, 1.0f, -1.0f, 0.0f, 1.0f),
		Vertex(-1.0f, 1.0f,  1.0f, 0.0f, 0.0f),
		Vertex(1.0f, 1.0f,  1.0f, 1.0f, 0.0f),
		Vertex(1.0f, 1.0f, -1.0f, 1.0f, 1.0f),

		// Bottom Face
		Vertex(-1.0f, -1.0f, -1.0f, 1.0f, 1.0f),
		Vertex(1.0f, -1.0f, -1.0f, 0.0f, 1.0f),
		Vertex(1.0f, -1.0f,  1.0f, 0.0f, 0.0f),
		Vertex(-1.0f, -1.0f,  1.0f, 1.0f, 0.0f),

		// Left Face
		Vertex(-1.0f, -1.0f,  1.0f, 0.0f, 1.0f),
		Vertex(-1.0f,  1.0f,  1.0f, 0.0f, 0.0f),
		Vertex(-1.0f,  1.0f, -1.0f, 1.0f, 0.0f),
		Vertex(-1.0f, -1.0f, -1.0f, 1.0f, 1.0f),

		// Right Face
		Vertex(1.0f, -1.0f, -1.0f, 0.0f, 1.0f),
		Vertex(1.0f,  1.0f, -1.0f, 0.0f, 0.0f),
		Vertex(1.0f,  1.0f,  1.0f, 1.0f, 0.0f),
		Vertex(1.0f, -1.0f,  1.0f, 1.0f, 1.0f),
	};

	nVertices = ARRAYSIZE(vertices);

	// sizeof(vertices) / sizeof(vertices[0]);
	// ���ؽ� ���� ������(Descriptor).
	D3D11_BUFFER_DESC vbDesc;
	ZeroMemory(&vbDesc, sizeof(D3D11_BUFFER_DESC));
	vbDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbDesc.ByteWidth = sizeof(Vertex) * ARRAYSIZE(vertices);
	vbDesc.CPUAccessFlags = 0;
	vbDesc.MiscFlags = 0;
	vbDesc.Usage = D3D11_USAGE_DEFAULT;

	// ���� ���ҽ� ����.
	D3D11_SUBRESOURCE_DATA vbData;
	ZeroMemory(&vbData, sizeof(D3D11_SUBRESOURCE_DATA));
	vbData.pSysMem = vertices;

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
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
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

	// ����Ʈ ����.
	pDeviceContext->RSSetViewports(1, &viewport);
}

// ���� ��ȯ ��� ����.
void DXApp::InitTransformMatrix()
{
	// World x View x Projection;
	// ���� ���. (��ġ/ȸ��/������).
	world = XMMatrixIdentity();

	// ī�޶� ��ġ.
	cameraPos = XMVectorSet(3.0f, 4.0f, -7.0f, 0.0f);

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
	// ���̴� ������.
	if (!ShaderCompile())
		return false;

	// ���� ���� ����.
	InitVertexBuffer();

	// �ε��� ���� ����.
	InitIndexBuffer();

	// �Է� ���̾ƿ� ����.
	if (!InitInputLayout())
		return false;

	// �⺻ �������� ���� (TRIANGLELIST�� ����).
	pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// ����Ʈ ����.
	SetViewport();

	// ���� ��ȯ ��� ����.
	InitTransformMatrix();

	// �ؽ�ó �ε�(���÷� �ʱ�ȭ).
	if (!InitTexture())
		return false;

	// �ؽ�ó ���ε�.
	pDeviceContext->PSSetShaderResources(0, 1, &pTexture);
	// ���÷� ������Ʈ ���ε�.
	pDeviceContext->PSSetSamplers(0, 1, &pTextureSamplerState);

	// �����Ͷ����� ���� ����.
	if (SetRasterizerState() == false)
		return false;

	return true;
}

bool DXApp::InitTexture()
{
	HRESULT hr;
	// �ؽ�ó ���� �ε�.
	hr = D3DX11CreateShaderResourceViewFromFile(pDevice,
		L"dx.jpg", NULL, NULL, &pTexture, NULL);

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