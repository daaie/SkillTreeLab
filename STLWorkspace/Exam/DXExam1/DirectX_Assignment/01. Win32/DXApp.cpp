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
	appTitle = L"01. Win32";
	wndStyle = WS_OVERLAPPEDWINDOW;
	g_pApp = this;
}

DXApp::~DXApp()
{

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