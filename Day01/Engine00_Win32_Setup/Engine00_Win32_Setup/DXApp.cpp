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
	wndStyle= WS_OVERLAPPEDWINDOW; //������ ���� �ɼ� WS_ ��¼����¼��-> â ��Ÿ��? ������ ���־��°� ������ f12������...
	g_pApp = this;
}

DXApp::~DXApp()
{
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
		//GetMessage(); �޼����� �����µ��� �ٸ� ó���¸��� �޼��� ó�� ������ �� 
		//PeekMessage(); ó���� �޼��� ���ϰ� �������� �����ϰ� DX������ �� ��
		if (PeekMessage(&msg, NULL, NULL, NULL, PM_REMOVE)) //�޼��� ó���ϰ� �����ų� �����ų�..PM_REMOVE �� ó���� ������.
		{
			// �޽��� �ؼ�
			TranslateMessage(&msg);
			// �޽��� ����.
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

	return (int)msg.wParam; //������ â���鶧 ����?������.
}

bool DXApp::Init()
{
	if (InitWindow() == false)
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
	wc.cbSize = sizeof(WNDCLASSEX); // ������ Ŭ������ ex, w�� ������ �پ��ؼ� ����� �Ѱ���
	wc.hbrBackground = NULL;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION); // ������ ���ҽ��� �޾ƿͼ� ��δ�
	wc.lpszClassName = "WindowClass"; // â���� Ŭ���� ���� �����Ҽ��ִ�
	wc.lpszMenuName = NULL;
	//DXApp::MsgProc �̷��Դ� ���� ���� ��ܿ� ��ȸ�س���..
	wc.lpfnWndProc = MainMsgProc;

	//������ ���
	if (RegisterClassEx(&wc) == false)
		return false;

	// ������ ����
	// Ŭ���� �������� ã�Ƽ� ������ ������Ŭ������ �̸� lpszClassName �� �����ؼ� �־����
	hwnd = CreateWindow("WindowClass", appTitle,
		wndStyle, 0, 0, clientWidth, clientHeight, 
		NULL, NULL, hinstance, NULL);
	
	if (hwnd == NULL)
		return false;

	//�Ϲ����� �׳� ���� �ɼ� SHOW
	ShowWindow(hwnd, SW_SHOW);

	return true;
}

// �����찡 ���α׷��� �˷��ִ� �Լ�.. �ݹ��̶�� �θ�. 
// �̺�Ʈ�� �߻��ϸ� Run���� ����� �˷���.
// �޽����� ����..Run���� peek translate dispatch �ߵ�������
LRESULT DXApp::MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_KEYDOWN:
		// escŰ ���ȴ��� Ȯ��
		if (wParam == VK_ESCAPE)
		{
			// �������� �����ڵ�� ���ڿ��տ� L�ٿ�����
			// MB_ICONQUESTION�� ������ ? �߰��ϴ°�
			if (MessageBox(NULL, "�����Ͻðڽ��ϱ�", "����",
				MB_YESNO | MB_ICONQUESTION) == IDYES)
			{
				// ��Ű ������ â ����
				DestroyWindow(this->hwnd);
			}
			return 0;
		}
		else if (wParam == VK_SPACE)
		{
			MessageBox(NULL, "�־ȵ�", "pause", MB_OK);
		}
		break;
		// â�� �����Ǹ� ����
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
		//->WM_QUIT ���� RUN()�� �� -> ���� ����
		break;

		//�����쿡�� �����ϴ� ����Ʈ ���ν���
	default: return DefWindowProc(hwnd, msg, wParam, lParam);
	}

	return LRESULT();
}
