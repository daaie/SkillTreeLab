#include "DXApp.h"

DXApp* g_pApp = NULL;

LRESULT CALLBACK MainMsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (g_pApp) return g_pApp->MsgProc(hwnd, msg, wParam, lParam);

	else return DefWindowProc(hwnd, msg, wParam, lParam);
}

DXApp::DXApp(HINSTANCE hinstance)
{
	hwnd = NULL;
	this->hinstance = hinstance;
	clientWidth = 800;
	clientHeight = 600;
	appTitle = L"03. Direct3D_BG";
	wndStyle = WS_OVERLAPPEDWINDOW;
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
	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, NULL, NULL, NULL, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			Update();
			Render();
		}
	}

	return (int)msg.wParam;
}

bool DXApp::Init()
{
	if (InitWindow() == false)
		return false;

	if (InitDirect3D() == false)
		return false;

	return true;
}

bool DXApp::InitWindow()
{
	WNDCLASSEX wc;
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

	if (RegisterClassEx(&wc) == false)
		return false;

	hwnd = CreateWindow(L"WindowClass", appTitle,
		wndStyle, 0, 0, clientWidth, clientHeight,
		NULL, NULL, hinstance, NULL);

	if (hwnd == NULL)
	{
		return false;
	}

	ShowWindow(hwnd, SW_SHOW);

	return true;
}

bool DXApp::InitDirect3D()
{
	DXGI_SWAP_CHAIN_DESC swapDesc;
	ZeroMemory(&swapDesc, sizeof(DXGI_SWAP_CHAIN_DESC));
	swapDesc.BufferCount = 1;
	swapDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapDesc.OutputWindow = hwnd;
	swapDesc.Windowed = true;
	swapDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	swapDesc.BufferDesc.Width = clientWidth;
	swapDesc.BufferDesc.Height = clientHeight;
	swapDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapDesc.BufferDesc.RefreshRate.Numerator = 60;
	swapDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapDesc.SampleDesc.Count = 1;
	swapDesc.SampleDesc.Quality = 0;

	HRESULT hr;
	hr = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, NULL, NULL, NULL, D3D11_SDK_VERSION, &swapDesc, &pSwapChain, 
		&pDevice, NULL, &pDeviceContext);

	if (FAILED(hr))
	{
		MessageBox(NULL, L"D3D11CreateDeviceAndSwapChain 실패", L"오류", MB_OK);
		return false;
	}

	ID3D11Texture2D* pBackBufferTexture;
	hr = pSwapChain->GetBuffer(NULL, __uuidof(ID3D11Texture2D),
		(void**)&pBackBufferTexture);

	if (FAILED(hr))		
	{
		MessageBox(NULL, L"pSwapChain->GetBuffer 실패", L"오류", MB_OK);
		return false;
	}

	// 렌더 타켓 뷰 생성.
	hr = pDevice->CreateRenderTargetView(pBackBufferTexture, NULL,
		&pRenderTargetView);
	
	if (FAILED(hr))		
	{
		MessageBox(NULL, L"pDevice->CreateRenderTargetView 실패", L"오류", MB_OK);
		return false;
	}

	pDeviceContext->OMSetRenderTargets(1, &pRenderTargetView, NULL);

	Memory::SafeRelease(pBackBufferTexture);

	return true;
}

LRESULT DXApp::MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE)
		{
			if (MessageBox(NULL, L"종료하시겠습니까?", L"종료", MB_YESNO | MB_ICONQUESTION) == IDYES)
			{
				DestroyWindow(this->hwnd);
			}
		}
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

	default: return DefWindowProc(hwnd, msg, wParam, lParam);
	}
}