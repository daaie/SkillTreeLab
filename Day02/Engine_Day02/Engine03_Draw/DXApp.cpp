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
	hwnd = NULL; //Ÿ�Ը𸣸� f12Ÿ�� ������ ��..
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
	// adapter -> NULL �̸� �⺻ ��ġ ���, ���� �׷��� 2���̸� ���������ϴ�
	// Device, Device Context �ʱ�ȭ�Լ� -> D3D11CreateDevice(NULL, );

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
	swapDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;// ������ unsigned normalize 0~1
	swapDesc.BufferDesc.RefreshRate.Numerator = 60; // ����
	swapDesc.BufferDesc.RefreshRate.Denominator = 1; // �и� // 1�п� 60��

								   //�ȼ� ������ -> ��ȭ == ����ǥ��ȭ?
	swapDesc.SampleDesc.Count = 1; // 1�̸� ���ϰڴ�,, ī��Ʈ�� �ø� �ֺ� �ؽ��İ��� �̿��ϰڴ�
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
	//��ġ�� ����ü�μ��� 
	hr=D3D11CreateDeviceAndSwapChain(NULL,D3D_DRIVER_TYPE_HARDWARE, // ����� �����ϸ� �ϵ���� ���Ӱ����ϴ� �ȵǸ� ����Ʈ�����ؾ�����..
								NULL, NULL, NULL, NULL,D3D11_SDK_VERSION,
								&swapDesc,&pSwapChain,&pDevice,NULL,&pDeviceContext); 
	
	// if (SUCCEEDED(hr)) // �������� S_OK
	if (FAILED(hr)) // ���п��� E_FAIL
	{
		MessageBox(NULL, "��ġ���� ����","����",MB_OK);
		return false;
	}

	// 2D �ؽ��� ����(���� Ÿ�� ��)
	// Texutre2D�� ID311Resource (������Ʈ��, �ؽ���, ���̴�) �� ���..
	ID3D11Texture2D* pBackBufferTexture;
	// ����ü������ �ؽ�ó �������� �ϴ� ����, ����ۿ� �׸��׸��� ����

	// pBackBufferTexture�� �׷��� ���ҹ��� ����..���ҽ�Ư�������� �Ⱦ��Ǵ� �������.
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
	// ���� Ÿ�� ��� �������ϼ� �ִ�. ȭ��ǥ���� 4���� �Ҽ����ְ�
	// ����۰� 3�� 4���ϼ����ְ�, ù���� ���ڰ� �� ������
	//Output Merge set OMSet
	pDeviceContext->OMSetRenderTargets(1, &pRenderTargetView,NULL);

	//3. ���ҽ� ����..���ҽ��� pBackBufferTexture�ְ� ���������ϱ�
	Memory::SafeRelease(pBackBufferTexture);


	return true;
}

bool DXApp::InitScene()
{   //MS�� hlsl -> high level shader level ..�����ƴ� cg��� ���̴� ��� //unity�� SOL �� �ڵ��ϴµ� CG, HLSL �Ѵ� ����
	//�ڵ忡�� ���̴� ������ , ���̴� ���� �Ӽ� ->> �׸����� -> ���忡 �������� �ϱ��;���
	//�ι��� ���� ��ũ�δ� �������Ѱ� 
	//vs_5_0�� ���̴� �������� ....dx lib�� ���������..�����ϴ���...

	HRESULT hr;

	hr = D3DX11CompileFromFile("Effects.fx", NULL, NULL, "VS_Main", "vs_5_0", NULL, NULL, NULL, &vertextShaderBuffer, NULL, NULL);
	if (FAILED(hr))
	{
		MessageBox(NULL, "D3DX11CompileFromFile111 Fail", "����", MB_OK);
		return false;
	}

	// vertextShaderBuffer�� ���̴� �������ϵ� ����Ʈ�ڵ� -> VS ������Ʈ���� -> ���������ο� ���ε�

	// ���� ���̴� ������Ʈ ����
	pDevice->CreateVertexShader(vertextShaderBuffer->GetBufferPointer(), // gpu�� �޸𸮰����� �̿��ϴ°Ŷ� gpu�� �ּҰ��̶� ������ ��û��
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

	// ���� �����.
	// 1. vertex ����ü ����.
	// z�� +�̸� ȭ�鿡�� �־���
	// vertext�� �޼���ǥ��cw�� ���󰡾��� ���� �հ��� ���⸸ �׸�..�ڴ¾ȱ׸�..�ȳ���

	Vertex vertices[] = { //0~1
		Vertex(0.0f,1.0f,0.5f),
		Vertex(1.0f,-1.0f,0.5f),
		Vertex(-1.0f,-1.0f,0.5f)
	};

	// 2. ���� ���� ( ���� desc, ���� ���ҽ�. )
	D3D11_BUFFER_DESC vbDesc; // ������
	ZeroMemory(&vbDesc, sizeof(D3D11_BUFFER_DESC));

	vbDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbDesc.ByteWidth = sizeof(Vertex) * ARRAYSIZE(vertices);
	vbDesc.CPUAccessFlags = 0; // CPU GPU ������.. CPU���� �����ؼ� �ٻ��
	vbDesc.MiscFlags = 0;
	vbDesc.Usage = D3D11_USAGE_DEFAULT; 
	// ���� ���̳����� ���� ���ε��ҋ� �������� BINDFlags ������..
	// ���⼭�� BINDFlags �� VERTEX_BUFFER�� �������־ �׳� �������Ŵ�.
	
	// ���� ���ҽ� ���� - vertex ����
	D3D11_SUBRESOURCE_DATA vbData;
	ZeroMemory(&vbData, sizeof(D3D11_SUBRESOURCE_DATA));
	vbData.pSysMem = vertices;

	// buffer ���ҽ� vertexBuffer�� ���긮�ҽ����� �������� �����ִ�.
	pDevice->CreateBuffer(&vbDesc, &vbData, &vertexBuffer);

	// ���� ���� ���ε�
	UINT stride = sizeof(Vertex);
	UINT offset = 0;

	//��ǲ�����(IA)���� �˷�����Ѵ� ���ؽ����� - ������Ʈ������
	pDeviceContext->IASetVertexBuffers(0,1,&vertexBuffer, &stride, &offset);

	// 11���� �Է� ���̾ƿ� , ��� vertex�� �ؼ����� �˷�����Ѵ�.
	// layout ���� �����ϴ� ���� = Vertext������ ������� ���� ���ƾ���..
	D3D11_INPUT_ELEMENT_DESC layout[] = {
		{"POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0},
	};

	pDevice->CreateInputLayout(layout, ARRAYSIZE(layout),
							vertextShaderBuffer->GetBufferPointer(), 
							vertextShaderBuffer->GetBufferSize(), 
							&vertexInputLayout);

	pDeviceContext->IASetInputLayout(vertexInputLayout);

	
	// �⺻ ��������(����) ����.(TRIANGLELIST�� ����) -> ������ ���Ұ��� ���������.
	// �ﰡ���� list �ƴϸ� srip�� ���̾� 
	// list�� ���� ����Ʈ 3���� ���ʿ��� 
	// strip�� ������ �����Ҽ����� ���ߴ¹���̴ٸ���.
	// �ᱹ �ؽ��ĸ� �����ҷ��� �� �ﰢ���� ������ �� �ʿ��ϴϱ� list�ؾ���
	pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// ����Ʈ ���� - 9������ �⺻ ����Ʈ���ִµ� ����¾�������
	// ��������
	D3D11_VIEWPORT viewport;
	ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));
	viewport.TopLeftX = 200;
	viewport.TopLeftY = 150;
	viewport.Width = clientWidth *0.5;
	viewport.Height = clientHeight*0.5;

	pDeviceContext->RSSetViewports(1, &viewport);

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
