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
	//메인 메시지 처리.
	//MSG msg ={0};
	MSG msg;
	ZeroMemory(&msg, sizeof(MSG)); // 초기화

	while (msg.message != WM_QUIT)
	{
		//메시지 처리 
		if (PeekMessage(&msg, NULL, NULL, NULL, PM_REMOVE))
		{
			TranslateMessage(&msg);
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

	return (int)msg.wParam; 
}

bool DXApp::Init()
{
	//창만들고
	if (InitWindow() == false)
	{
		return false;
	}

	//directX 초기화
	if (InitDirect3D() == false)
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
	wc.cbSize = sizeof(WNDCLASSEX); 
	wc.hbrBackground = NULL;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO); // 아이콘 리소스에 받아와서 써두댐
	wc.lpszClassName = "WindowClass";
	wc.lpszMenuName = NULL; //메뉴를 사용안함
	//DXApp::MsgProc 이렇게는 못씀 제일 상단에 우회해놓음..
	wc.lpfnWndProc = MainMsgProc;
	//wc.hInstance = hinstance; -> //디폴트는 현재 실행중인거
	wc.style = CS_CLASSDC; // 클래스스타일

	if (RegisterClassEx(&wc) == false)
		return false;

	// 클레스 네임으로 찾아서 생성함 윈도우클래스의 이름 lpszClassName 잘 복사해서 넣어줘라
	hwnd = CreateWindow("WindowClass", appTitle,
		wndStyle, 0, 0, clientWidth, clientHeight, 
		NULL, NULL, hinstance, NULL);
		
	if (hwnd == NULL)
		return false;

	//일반으로 그냥 띄우는 옵션 SHOW
	ShowWindow(hwnd, SW_SHOW);

	//창 갱신
	UpdateWindow(hwnd);

	return true;
}

// 장치, 스왑체인 초기화
bool DXApp::InitDirect3D()
{ 
	// adapter -> NULL 이면 기본 장치 사용, 만약 그래픽 2개이면 지정가능하다
	// Device, Device Context 초기화함수 -> D3D11CreateDevice(NULL, );

	// 스왑 체인 정보 설정 -> 서술자 설정
	DXGI_SWAP_CHAIN_DESC swapDesc; 
	ZeroMemory(&swapDesc,sizeof(DXGI_SWAP_CHAIN_DESC));
	swapDesc.BufferCount = 1; // 백버퍼 개수.. 0이면 백버퍼 없는 거 1이면 이중버퍼링
	swapDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; // 버퍼 용도 
	swapDesc.OutputWindow = hwnd;
	swapDesc.Windowed = true; // 시작할떄는 전체화면모드 ㄴㄴ 윈도우드 모드..
	swapDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD; //설정안하는게 낫다..ㅋㅋ
	swapDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH; // 화면크기 변경 되면 갱신
	swapDesc.BufferDesc.Width = clientWidth; //백버퍼 해상도 -> 윈도우와 동일
	swapDesc.BufferDesc.Height = clientHeight;//백버퍼 해상도
	swapDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;// 색상값이 unsigned normalize 0~1
	swapDesc.BufferDesc.RefreshRate.Numerator = 60; // 분자
	swapDesc.BufferDesc.RefreshRate.Denominator = 1; // 분모 // 1분에 60번

								   //픽셀 점점점 -> 완화 == 다중표본화?
	swapDesc.SampleDesc.Count = 1; // 1이면 안하겠다,, 카운트가 늘면 주변 텍스쳐값을 이용하겠다
	swapDesc.SampleDesc.Quality = 0; 
	
	// feature level dx버전 다지원하는 겜을 만들고싶다 -> 일케배열만들고 -> 있는지 물어본다.찾아봥..
	D3D_FEATURE_LEVEL featureLevels[] = {
		D3D_FEATURE_LEVEL_9_1,
		D3D_FEATURE_LEVEL_9_2,
		D3D_FEATURE_LEVEL_9_3,
		D3D_FEATURE_LEVEL_10_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_11_0
	};

	HRESULT hr;
	//장치및 스왑체인설정 
	hr=D3D11CreateDeviceAndSwapChain(NULL,D3D_DRIVER_TYPE_HARDWARE, // 요새는 웬만하면 하드웨어 가속가능하다 안되면 소프트웨어해야지뭐..
								NULL, NULL, NULL, NULL,D3D11_SDK_VERSION,
								&swapDesc,&pSwapChain,&pDevice,NULL,&pDeviceContext); 
	
	// if (SUCCEEDED(hr)) // 성공여부 S_OK
	if (FAILED(hr)) // 실패여부 E_FAIL
	{
		MessageBox(NULL, "장치생성 실패","오류",MB_OK);
		return false;
	}

	// 2D 텍스쳐 생성(렌더 타겟 용)
	//Texutre2D는 ID311Resource (지오메트리, 텍스쳐, 쉐이더) 를 상속..
	ID3D11Texture2D* pBackBufferTexture;
	// 스왑체인한테 텍스처 저장해죠 하는 과정, 백버퍼에 그림그리는 과정

	// pBackBufferTexture에 그래픽 스왑버퍼 얻음..리소스특성에따라 안얻어도되는 경우있음.
	hr = pSwapChain->GetBuffer(NULL, __uuidof(ID3D11Texture2D),(void**)&pBackBufferTexture);
	if (FAILED(hr))
	{
		MessageBox(NULL, "get swap buffer failed", "오류", MB_OK);
		return false;
	}

	// 1. 렌더타겟 뷰 생성
	hr = pDevice->CreateRenderTargetView(pBackBufferTexture, NULL, &pRenderTargetView);
	if (FAILED(hr))
	{
		MessageBox(NULL, "CreateRenderTargetView failed", "오류", MB_OK);
		return false;
	}
	// 2. 렌더 타겟 뷰 바인딩 , 파이프 라인에 연결.
	// 렌더 타겟 뷰는 여러개일수 있다. 화면표현을 4개로 할수도있고
	// 백버퍼가 3개 4개일수도있고, 첫번쨰 인자가 그 개수임
	//Output Merge set OMSet
	pDeviceContext->OMSetRenderTargets(1, &pRenderTargetView,NULL);

	//3. 리소스 해제..리소스를 pBackBufferTexture애가 물고있으니까
	Memory::SafeRelease(pBackBufferTexture);
	return true;
}

LRESULT DXApp::MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_KEYDOWN:
		// esc키 눌렸늦지 확인
		if (wParam == VK_ESCAPE)
		{
			if (MessageBox(NULL, "종료하시겠습니까", "종료",
				MB_YESNO | MB_ICONQUESTION) == IDYES)
			{
				DestroyWindow(hwnd); //여기서는 hwnd == this->hwnd
			}
			return 0;
		}
		else if (wParam == VK_SPACE)
		{
			MessageBox(NULL, "왜안돼", "pause", MB_OK);
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);//->WM_QUIT 으로 RUN()에 감 -> 와일 꺠짐
		return 0;

	default: return DefWindowProc(hwnd, msg, wParam, lParam);
	}

	return LRESULT();
}
