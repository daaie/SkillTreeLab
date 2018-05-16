#include "DXApp.h"

// 메시지 프로시저 설정을 우회하는데 사용하기 위해 생성.
DXApp* g_pApp = NULL;

LRESULT CALLBACK MainMsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (g_pApp)
	{
		//g_pAPP 변수가 설정되어있으면 MsgProc 사용.
		return g_pApp->MsgProc(hwnd, msg, wParam, lParam);
	}
	else 
		// 설정되지 않으면 윈도우 기본 프로시저 함수 호출.
		return DefWindowProc(hwnd, msg, wParam, lParam);
}

DXApp::DXApp(HINSTANCE hinstance)
{
	hwnd = NULL; //타입모르면 f12타고 가ㅅㅓ 봐..
	this->hinstance = hinstance;
	clientWidth = 800;
	clientHeight = 600;
	appTitle = "Engine00_Win32_Setup";
	wndStyle= WS_OVERLAPPEDWINDOW; //윈도우 띄울떄의 옵션 WS_ 어쩌구저쩌구-> 창 스타일? 같은거 자주쓰는거 묶어논거 f12가서봐...
	g_pApp = this;
}

DXApp::~DXApp()
{
}

int DXApp::Run()
{
	//메인 메시지 처리.
	//MSG msg ={0};
	MSG msg;
	ZeroMemory(&msg, sizeof(MSG)); // 초기화

	while (msg.message != WM_QUIT)
	{
		//메시지 처리 
		//GetMessage(); 메세지가 들어오는동안 다른 처리는못함 메세지 처리 루프만 씀 
		//PeekMessage(); 처리할 메세지 픽하고 나머지는 실행하게 DX에서는 얠 씀
		if (PeekMessage(&msg, NULL, NULL, NULL, PM_REMOVE)) //메세지 처리하고 버릴거냐 가질거냐..PM_REMOVE 는 처리후 버린다.
		{
			// 메시지 해석
			TranslateMessage(&msg);
			// 메시지 전달.
			DispatchMessage(&msg);
		}
		//DX 렌더링 루프
		else
		{
			// Update Scene.
			Update();

			// Render Scene.
			Render();
		}
	}

	return (int)msg.wParam; //윈도우 창만들때 관습?ㅋㅋㅋ.
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
	// 윈도우 클래스 값 생성/설정.
	WNDCLASSEX wc; // windowClass
	// 구조체 초기화.
	ZeroMemory(&wc, sizeof(WNDCLASSEX));

	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.cbSize = sizeof(WNDCLASSEX); // 윈도우 클래스가 ex, w등 버전이 다양해서 사이즈를 넘겨줌
	wc.hbrBackground = NULL;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION); // 아이콘 리소스에 받아와서 써두댐
	wc.lpszClassName = "WindowClass"; // 창별로 클래스 네임 지정할수있다
	wc.lpszMenuName = NULL;
	//DXApp::MsgProc 이렇게는 못씀 제일 상단에 우회해놓음..
	wc.lpfnWndProc = MainMsgProc;

	//윈도우 등록
	if (RegisterClassEx(&wc) == false)
		return false;

	// 윈도우 생성
	// 클레스 네임으로 찾아서 생성함 윈도우클래스의 이름 lpszClassName 잘 복사해서 넣어줘라
	hwnd = CreateWindow("WindowClass", appTitle,
		wndStyle, 0, 0, clientWidth, clientHeight, 
		NULL, NULL, hinstance, NULL);
	
	if (hwnd == NULL)
		return false;

	//일반으로 그냥 띄우는 옵션 SHOW
	ShowWindow(hwnd, SW_SHOW);

	return true;
}

// 윈도우가 프로그램에 알려주는 함수.. 콜백이라고 부름. 
// 이벤트가 발생하면 Run에서 여기로 알려줌.
// 메시지가 들어옴..Run에서 peek translate dispatch 잘도ㅐ야함
LRESULT DXApp::MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_KEYDOWN:
		// esc키 눌렸늦지 확인
		if (wParam == VK_ESCAPE)
		{
			// 문자집합 유니코드면 문자열앞에 L붙여야함
			// MB_ICONQUESTION는 아이콘 ? 추가하는거
			if (MessageBox(NULL, "종료하시겠습니까", "종료",
				MB_YESNO | MB_ICONQUESTION) == IDYES)
			{
				// 예키 눌리면 창 삭제
				DestroyWindow(this->hwnd);
			}
			return 0;
		}
		else if (wParam == VK_SPACE)
		{
			MessageBox(NULL, "왜안돼", "pause", MB_OK);
		}
		break;
		// 창이 삭제되면 실행
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
		//->WM_QUIT 으로 RUN()에 감 -> 와일 꺠짐
		break;

		//윈도우에서 제공하는 디폴트 프로시져
	default: return DefWindowProc(hwnd, msg, wParam, lParam);
	}

	return LRESULT();
}
