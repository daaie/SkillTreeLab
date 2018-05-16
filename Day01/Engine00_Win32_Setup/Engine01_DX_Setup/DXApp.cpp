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

}

DXApp::~DXApp()
{
	Memory::SafeRelease(pDevice);
	Memory::SafeRelease(pDeviceContext);
	Memory::SafeRelease(pSwapChain);
	Memory::SafeRelease(pRenderTargetView);
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
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO); // ������ ���ҽ��� �޾ƿͼ� ��δ�
	wc.lpszClassName = "WindowClass";
	wc.lpszMenuName = NULL; //�޴��� ������
	//DXApp::MsgProc �̷��Դ� ���� ���� ��ܿ� ��ȸ�س���..
	wc.lpfnWndProc = MainMsgProc;
	//wc.hInstance = hinstance; -> //����Ʈ�� ���� �������ΰ�
	wc.style = CS_CLASSDC; // Ŭ������Ÿ��

	if (RegisterClassEx(&wc) == false)
		return false;

	// Ŭ���� �������� ã�Ƽ� ������ ������Ŭ������ �̸� lpszClassName �� �����ؼ� �־����
	hwnd = CreateWindow("WindowClass", appTitle,
		wndStyle, 0, 0, clientWidth, clientHeight, 
		NULL, NULL, hinstance, NULL);
		
	if (hwnd == NULL)
		return false;

	//�Ϲ����� �׳� ���� �ɼ� SHOW
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
	//Texutre2D�� ID311Resource (������Ʈ��, �ؽ���, ���̴�) �� ���..
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
