#include "DXApp.h"

// �޽��� ���ν��� ������ ��ȸ�ϴµ� ����ϱ� ���� ����.
DXApp* g_pApp = NULL;

LRESULT CALLBACK MainMsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (g_pApp)
	{
		//g_pAPP ������ �����Ǿ������� MsgProc ���.
		return g_pApp->MsgProc(hwnd, msg, wParam, lParam);
	}
	else 
		// �������� ������ ������ �⺻ ���ν��� �Լ� ȣ��.
		return DefWindowProc(hwnd, msg, wParam, lParam);
}

DXApp::DXApp(HINSTANCE hinstance)
{
	hwnd = NULL;
	this->hinstance = hinstance;
	clientWidth = 800;
	clientHeight = 600;
	appTitle = "Engine00_Win32_Setup";
	wndStyle= WS_OVERLAPPEDWINDOW; 
	g_pApp = this;

	pDevice = NULL;
	pDeviceContext = NULL;
	pSwapChain = NULL;
	pRenderTargetView = NULL;


	vertexBuffer = NULL; 
	vertexShader = NULL; 
	pixelShader = NULL; 
	vertextShaderBuffer = NULL; 
	pixelShaderBuffer = NULL;
	vertexInputLayout = NULL;

	indexBuffer = NULL; 
	cbBuffer = NULL;
}

DXApp::~DXApp()
{
	Memory::SafeRelease(pDevice);
	Memory::SafeRelease(pDeviceContext);
	Memory::SafeRelease(pSwapChain);
	Memory::SafeRelease(pRenderTargetView);

	Memory::SafeRelease(vertexBuffer ); 
	Memory::SafeRelease(vertexShader ); 
	Memory::SafeRelease(pixelShader ); 
	Memory::SafeRelease(vertextShaderBuffer); 
	Memory::SafeRelease(pixelShaderBuffer );
	Memory::SafeRelease(vertexInputLayout);

	Memory::SafeRelease(indexBuffer);
	Memory::SafeRelease(cbBuffer);
}

int DXApp::Run()
{
	//���� �޽��� ó��.
	//MSG msg ={0};
	MSG msg;
	ZeroMemory(&msg, sizeof(MSG)); // �ʱ�ȭ

	while (msg.message != WM_QUIT)
	{
		//�޽��� ó�� 
		if (PeekMessage(&msg, NULL, NULL, NULL, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		//DX ������ ����
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
	//â�����
	if (InitWindow() == false)
	{
		return false;
	}

	//directX �ʱ�ȭ
	if (InitDirect3D() == false)
	{
		return false;
	}

	// ��� �ʱ�ȭ.
	if (InitScene() == false)
	{
		return false;
	}
	return true;
}


bool DXApp::InitWindow()
{
	// ������ Ŭ���� �� ����/����.
	WNDCLASSEX wc; // windowClass
	// ����ü �ʱ�ȭ.
	ZeroMemory(&wc, sizeof(WNDCLASSEX));

	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.cbSize = sizeof(WNDCLASSEX); 
	wc.hbrBackground = NULL;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO); 
	wc.lpszClassName = "WindowClass";
	wc.lpszMenuName = NULL; 
	wc.lpfnWndProc = MainMsgProc;
	//wc.hInstance = hinstance; -> //����Ʈ�� ���� �������ΰ�
	wc.style = CS_CLASSDC; // Ŭ������Ÿ��

	if (RegisterClassEx(&wc) == false)
		return false;

	hwnd = CreateWindow("WindowClass", appTitle,
		wndStyle, 0, 0, clientWidth, clientHeight, 
		NULL, NULL, hinstance, NULL);
		
	if (hwnd == NULL)
		return false;

	ShowWindow(hwnd, SW_SHOW);

	//â ����
	UpdateWindow(hwnd);

	return true;
}

// ��ġ, ����ü�� �ʱ�ȭ
bool DXApp::InitDirect3D()
{ 
	// ���� ü�� ���� ���� -> ������ ����
	DXGI_SWAP_CHAIN_DESC swapDesc; 
	ZeroMemory(&swapDesc,sizeof(DXGI_SWAP_CHAIN_DESC));
	swapDesc.BufferCount = 1; // ����� ����.. 0�̸� ����� ���� �� 1�̸� ���߹��۸�
	swapDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; // ���� �뵵 
	swapDesc.OutputWindow = hwnd;
	swapDesc.Windowed = true; // �����ҋ��� ��üȭ���� ���� ������� ���..
	swapDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD; //�������ϴ°� ����..����
	swapDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH; // ȭ��ũ�� ���� �Ǹ� ����
	swapDesc.BufferDesc.Width = clientWidth; //����� �ػ� -> ������� ����
	swapDesc.BufferDesc.Height = clientHeight;//����� �ػ�
	swapDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapDesc.BufferDesc.RefreshRate.Numerator = 60; 
	swapDesc.BufferDesc.RefreshRate.Denominator = 1; 

	swapDesc.SampleDesc.Count = 1; 
	swapDesc.SampleDesc.Quality = 0; 
	
	// feature level dx���� �������ϴ� ���� �����ʹ� -> ���ɹ迭����� -> �ִ��� �����.ã�Ɣg..
	D3D_FEATURE_LEVEL featureLevels[] = {
		D3D_FEATURE_LEVEL_9_1,
		D3D_FEATURE_LEVEL_9_2,
		D3D_FEATURE_LEVEL_9_3,
		D3D_FEATURE_LEVEL_10_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_11_0
	};

	HRESULT hr;

	// ��ġ�� ����ü�μ��� 
	hr=D3D11CreateDeviceAndSwapChain(NULL,D3D_DRIVER_TYPE_HARDWARE, 
								NULL, NULL, NULL, NULL,D3D11_SDK_VERSION,
								&swapDesc,&pSwapChain,&pDevice,NULL,&pDeviceContext); 
	
	// if (SUCCEEDED(hr)) // �������� S_OK // ���п��� E_FAIL
	if (FAILED(hr)) 
	{
		MessageBox(NULL, "��ġ���� ����","����",MB_OK);
		return false;
	}

	// 2D �ؽ��� ����(���� Ÿ�� ��)
	// Texutre2D�� ID311Resource (������Ʈ��, �ؽ���, ���̴�) �� ���..
	ID3D11Texture2D* pBackBufferTexture;

	hr = pSwapChain->GetBuffer(NULL, __uuidof(ID3D11Texture2D),(void**)&pBackBufferTexture);
	if (FAILED(hr))
	{
		MessageBox(NULL, "get swap buffer failed", "����", MB_OK);
		return false;
	}

	// 1. ����Ÿ�� �� ����
	hr = pDevice->CreateRenderTargetView(pBackBufferTexture, NULL, &pRenderTargetView);
	if (FAILED(hr))
	{
		MessageBox(NULL, "CreateRenderTargetView failed", "����", MB_OK);
		return false;
	}

	// 2. ���� Ÿ�� �� ���ε� , ������ ���ο� ����.
	pDeviceContext->OMSetRenderTargets(1, &pRenderTargetView,NULL);

	// 3. ���ҽ� ����..���ҽ��� pBackBufferTexture�ְ� ���������ϱ�
	Memory::SafeRelease(pBackBufferTexture);


	return true;
}

bool DXApp::InitScene()
{  
	HRESULT hr;

	hr = D3DX11CompileFromFile("Effects.fx", NULL, NULL, "VS_Main", "vs_5_0", NULL, NULL, NULL, &vertextShaderBuffer, NULL, NULL);
	if (FAILED(hr))
	{
		MessageBox(NULL, "D3DX11CompileFromFile111 Fail", "����", MB_OK);
		return false;
	}

	// vertextShaderBuffer�� ���̴� �������ϵ� ����Ʈ�ڵ� -> VS ������Ʈ���� -> ���������ο� ���ε�

	// ���� ���̴� ������Ʈ ����
	pDevice->CreateVertexShader(vertextShaderBuffer->GetBufferPointer(),
							vertextShaderBuffer->GetBufferSize(), 
		 					NULL, &vertexShader);
	// ���� ���̴� ���ε�
	pDeviceContext->VSSetShader(vertexShader, NULL, NULL);


	// �ȼ� ���̴� ������ 
	hr = D3DX11CompileFromFile("Effects.fx", 0, 0, "PS_Main", "ps_5_0", 0, 0, 0, &pixelShaderBuffer, 0, 0);
	if (FAILED(hr))
	{
		MessageBox(NULL, "D3DX11CompileFromFile222 failed", "����", MB_OK);
		return false;
	}

	// �ȼ� ���̴� ������Ʈ����
	pDevice->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(),
		pixelShaderBuffer->GetBufferSize(), NULL, &pixelShader);
	
	// �ȼ� ���̴� ���ε�
	pDeviceContext->PSSetShader(pixelShader, NULL, NULL);

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

	// uv���� ���������� ������.
	// ���� �ȼ� ���̴��� 
	// ������ ������ �ܰ迡�� �ȼ��� ����� ���� ���� ���� 
	// 

	DWORD indices[] = {
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

	nIndices = ARRAYSIZE(indices);
	// ���� ���� ���� ( ���� desc, ���� ���ҽ�. )
	D3D11_BUFFER_DESC vbDesc;
	ZeroMemory(&vbDesc, sizeof(D3D11_BUFFER_DESC));

	vbDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbDesc.ByteWidth = sizeof(Vertex) * ARRAYSIZE(vertices);
	vbDesc.CPUAccessFlags = 0; 
	vbDesc.MiscFlags = 0;
	vbDesc.Usage = D3D11_USAGE_DEFAULT; 

	// ���� ���� ���� ���ҽ� ���� - vertex ����
	D3D11_SUBRESOURCE_DATA vbData;
	ZeroMemory(&vbData, sizeof(D3D11_SUBRESOURCE_DATA));
	vbData.pSysMem = vertices;

	// ���� buffer ���� // ���� ���ҽ� vertexBuffer�� ���긮�ҽ����� �������� �����ִ�.
	pDevice->CreateBuffer(&vbDesc, &vbData, &vertexBuffer);


	// �ε��� ���� ���� ( ���� desc, ���� ���ҽ�. )
	D3D11_BUFFER_DESC ibDesc;
	ZeroMemory(&ibDesc, sizeof(D3D11_BUFFER_DESC));
	ibDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibDesc.ByteWidth = sizeof(DWORD) * ARRAYSIZE(indices);
	ibDesc.CPUAccessFlags = 0;
	ibDesc.MiscFlags = 0;
	ibDesc.Usage = D3D11_USAGE_DEFAULT;

	// �ε��� ���� ���� ���ҽ� ���� - vertex ����
	D3D11_SUBRESOURCE_DATA ibData;
	ZeroMemory(&ibData, sizeof(D3D11_SUBRESOURCE_DATA));
	ibData.pSysMem = indices;

	// �ε������� ����
	pDevice->CreateBuffer(&ibDesc, &ibData, &indexBuffer);


	// ���� ���� ���ε�
	UINT stride = sizeof(Vertex);
	UINT offset = 0;

	// ���ؽ�����(������Ʈ������) ��ǲ�����(IA)���� �˷�����Ѵ� - ���������ο� �����°���
	pDeviceContext->IASetVertexBuffers(0,1,&vertexBuffer, &stride, &offset);

	// �ε��� ���� ���ε�
	pDeviceContext->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);


	D3D11_INPUT_ELEMENT_DESC layout[] = {
		{ "POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 },
		{ "TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,12,D3D11_INPUT_PER_VERTEX_DATA,0 }, 
		//TEXTURE�� �������� TEXCOORD0 ,1,2,3, ���ں��̸�ȴ�. ���̴� ������... ���̾ƿ������� ���̸� ��������.
	};

	pDevice->CreateInputLayout(layout, ARRAYSIZE(layout),
							vertextShaderBuffer->GetBufferPointer(), 
							vertextShaderBuffer->GetBufferSize(), 
							&vertexInputLayout);

	pDeviceContext->IASetInputLayout(vertexInputLayout);

	pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// ��������
	D3D11_VIEWPORT viewport;
	ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));
	viewport.TopLeftX =0;
	viewport.TopLeftY = 0;
	viewport.Width = clientWidth;
	viewport.Height = clientHeight;

	pDeviceContext->RSSetViewports(1, &viewport);

	// world * view * position
	// ���� ���. (��ü ��ġ/ȸ��/������)
	world = XMMatrixIdentity(); //���� ��� ������ִ� �Լ�
	// ���������� ī�޶� ȸ���ϴ� ���α׷��� ��� Update �Լ����� world ��������ָ� �ȴ�.
	// ���⼭�� �� ī�޶� �������ʳ�..
	
	// ī�޶� ��ġ
	cameraPos = XMVectorSet(3.0f, 4.0f, -7.0f, 0.0f);

	// ī�޶� �ٶ󺸴� ��ġ
	cameraTarget = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);

	// ī�޶� �� ���� ( ���⸸ �߿��ϹǷ� ���ڴ� �������)
	cameraUp = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	// �� ��� LH�� �޼���ǥ��
	view = XMMatrixLookAtLH(cameraPos, cameraTarget, cameraUp);

	// ���� ���
	// ���� �þ߰� - ���θ� ���߰� ���δ� ��Ⱦ�񿡵��� ���� 
	float fovY = 75.0f * 3.14 / 180.0f;	// �������� ���� �ѱ�

	// ī�޶� ���� - orthographic(2Dó��..), perspective(���ٹ�) ������� ..
	projection = XMMatrixPerspectiveFovLH(fovY,
			(float)clientWidth/clientHeight, 1.0f, 1000.0f);

	// ��� ���� ������ ���� �� ���������� ���ε�/
	// ��ġ ����� ���� ������ ä���
	// DX�� ��켱 , HLSL�� ���켱.
	CBPerObject data;
	data.world = XMMatrixTranspose(world);
	data.view = XMMatrixTranspose(view);
	data.projection = XMMatrixTranspose(projection);

	D3D11_BUFFER_DESC cbDesc;
	ZeroMemory(&cbDesc, sizeof(D3D11_BUFFER_DESC));

	cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbDesc.ByteWidth = sizeof(CBPerObject);
	cbDesc.CPUAccessFlags = 0;
	cbDesc.MiscFlags = 0;
	cbDesc.Usage = D3D11_USAGE_DEFAULT;


	// ���� ���ҽ�
	D3D11_SUBRESOURCE_DATA cbData;
	ZeroMemory(&cbData, sizeof(D3D11_SUBRESOURCE_DATA));
	cbData.pSysMem = &data;

	// ��� ���� ����
	pDevice->CreateBuffer(&cbDesc, &cbData, &cbBuffer);

	// ���������ο� ���ε�
	pDeviceContext->VSSetConstantBuffers(0, 1, &cbBuffer); // �Ѱ��� ���̴� �� �����ؼ� �Ѱ����

	//�ؽ��� �ε� �ʱ�ȭ
	if (!InitTexture())
		return false;
	
	//�ؽ��� �ȼ� ���̴� ������ ���ο� ����
	pDeviceContext->PSSetShaderResources(0, 1, &pTexture);

	// ���÷� ������Ʈ ���ε�
	pDeviceContext->PSSetSamplers(0, 1, &pTextureSamplerState);

	//dx���� �������Ŵ� �ּ��������ϰ� ���̴����� ó���� ���̴��� ��¥�� �Ѱ���
	return true;
}

bool DXApp::InitTexture()
{
	HRESULT hr;
	// �ؽ��� ���� �ε�
	hr = D3DX11CreateShaderResourceViewFromFile(pDevice,"dx.png",NULL,NULL,&pTexture,NULL);
	if (FAILED(hr))
	{
		MessageBox(NULL,"D3DX11CreateShaderResourceViewFromFile111", "���� ",MB_OK);
		return false;
	}

	D3D11_SAMPLER_DESC samplerDesc;
	ZeroMemory(&samplerDesc, sizeof(D3D11_SAMPLER_DESC));

	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	// wrap 0~1, 0~1
	// Mirror 0~1 1~0 
	// Clamp 0~1 1111
	// boarder 0~1 //////
	samplerDesc.AddressV= D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW= D3D11_TEXTURE_ADDRESS_WRAP;

	samplerDesc.MinLOD =0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	// Į���� ��� ������ �����ڷ� ���� -> ���������ο�
	hr = pDevice->CreateSamplerState(&samplerDesc, &pTextureSamplerState);
	if (FAILED(hr))
	{
		MessageBox(NULL, "CreateSamplerState", "���� ", MB_OK);
		return false;
	}

	return true;
}

LRESULT DXApp::MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_KEYDOWN:
		// escŰ ���ȴ��� Ȯ��
		if (wParam == VK_ESCAPE)
		{
			if (MessageBox(NULL, "�����Ͻðڽ��ϱ�", "����",
				MB_YESNO | MB_ICONQUESTION) == IDYES)
			{
				DestroyWindow(hwnd); //���⼭�� hwnd == this->hwnd
			}
			return 0;
		}
		else if (wParam == VK_SPACE)
		{
			MessageBox(NULL, "�־ȵ�", "pause", MB_OK);
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);//->WM_QUIT ���� RUN()�� �� -> ���� ����
		return 0;

	default: return DefWindowProc(hwnd, msg, wParam, lParam);
	}

	return LRESULT();
}
