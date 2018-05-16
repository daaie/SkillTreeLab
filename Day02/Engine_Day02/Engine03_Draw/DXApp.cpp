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

	// 장면 초기화.
	if (InitScene() == false)
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
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO); 
	wc.lpszClassName = "WindowClass";
	wc.lpszMenuName = NULL; 
	wc.lpfnWndProc = MainMsgProc;
	//wc.hInstance = hinstance; -> //디폴트는 현재 실행중인거
	wc.style = CS_CLASSDC; // 클래스스타일

	if (RegisterClassEx(&wc) == false)
		return false;

	hwnd = CreateWindow("WindowClass", appTitle,
		wndStyle, 0, 0, clientWidth, clientHeight, 
		NULL, NULL, hinstance, NULL);
		
	if (hwnd == NULL)
		return false;

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
	// Texutre2D는 ID311Resource (지오메트리, 텍스쳐, 쉐이더) 를 상속..
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

bool DXApp::InitScene()
{   //MS는 hlsl -> high level shader level ..엔비디아는 cg라는 쉐이더 언어 //unity는 SOL 로 코딩하는데 CG, HLSL 둘다 지원
	//코드에서 쉐이더 컴파일 , 쉐이더 파일 속성 ->> 항목형식 -> 빌드에 참여안함 하구와야함
	//두번쨰 인자 매크로는 디파인한거 
	//vs_5_0은 쉐이더 프로파일 ....dx lib에 물어봐야함..지원하는지...

	HRESULT hr;

	hr = D3DX11CompileFromFile("Effects.fx", NULL, NULL, "VS_Main", "vs_5_0", NULL, NULL, NULL, &vertextShaderBuffer, NULL, NULL);
	if (FAILED(hr))
	{
		MessageBox(NULL, "D3DX11CompileFromFile111 Fail", "오류", MB_OK);
		return false;
	}

	// vertextShaderBuffer는 쉐이더 ㅓ컴파일된 바이트코드 -> VS 오브젝트생성 -> 파이프라인에 바인딩

	// 정점 셰이더 오브젝트 생성
	pDevice->CreateVertexShader(vertextShaderBuffer->GetBufferPointer(), // gpu의 메모리공간을 이용하는거라서 gpu에 주소값이랑 사이즈 요청함
							vertextShaderBuffer->GetBufferSize(), 
		 					NULL, &vertexShader);
	// 정점 ㅖ이더 바인딩
	pDeviceContext->VSSetShader(vertexShader, NULL, NULL);


	// 픽셀 쉐이더 컴파일 
	hr = D3DX11CompileFromFile("Effects.fx", 0, 0, "PS_Main", "ps_5_0", 0, 0, 0, &pixelShaderBuffer, 0, 0);
	if (FAILED(hr))
	{
		MessageBox(NULL, "D3DX11CompileFromFile222 failed", "오류", MB_OK);
		return false;
	}

	// 픽셀 쉐이더 오브젝트생성
	pDevice->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(),
		pixelShaderBuffer->GetBufferSize(), NULL, &pixelShader);
	
	// 픽셀 쉐이더 바인딩
	pDeviceContext->PSSetShader(pixelShader, NULL, NULL);

	// 정점 만들기.
	// 1. vertex 구조체 정의.
	// z는 +이면 화면에서 멀어짐
	// vertext는 왼손좌표계cw를 따라가야함 엄지 손가락 방향만 그림..뒤는안그림..안나옴

	Vertex vertices[] = { //0~1
		Vertex(0.0f,1.0f,0.5f),
		Vertex(1.0f,-1.0f,0.5f),
		Vertex(-1.0f,-1.0f,0.5f)
	};

	// 2. 버퍼 생성 ( 버퍼 desc, 서브 리소스. )
	D3D11_BUFFER_DESC vbDesc; // 서술자
	ZeroMemory(&vbDesc, sizeof(D3D11_BUFFER_DESC));

	vbDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbDesc.ByteWidth = sizeof(Vertex) * ARRAYSIZE(vertices);
	vbDesc.CPUAccessFlags = 0; // CPU GPU 빡세게.. CPU에도 접근해서 다사용
	vbDesc.MiscFlags = 0;
	vbDesc.Usage = D3D11_USAGE_DEFAULT; 
	// 여기 다이나믹이 들어가면 바인딩할떄 동적으로 BINDFlags 설정함..
	// 여기서는 BINDFlags 가 VERTEX_BUFFER로 정해져있어서 그냥 정해질거다.
	
	// 서브 리소스 생성 - vertex 정보
	D3D11_SUBRESOURCE_DATA vbData;
	ZeroMemory(&vbData, sizeof(D3D11_SUBRESOURCE_DATA));
	vbData.pSysMem = vertices;

	// buffer 리소스 vertexBuffer는 서브리소스들의 집합으로 볼수있다.
	pDevice->CreateBuffer(&vbDesc, &vbData, &vertexBuffer);

	// 정점 버퍼 바인딩
	UINT stride = sizeof(Vertex);
	UINT offset = 0;

	//인풋어셈블러(IA)한테 알려줘야한다 버텍스정보 - 지오메트리정보
	pDeviceContext->IASetVertexBuffers(0,1,&vertexBuffer, &stride, &offset);

	// 11부터 입력 레이아웃 , 어떻게 vertex를 해석할지 알려줘야한다.
	// layout 여기 나열하는 순서 = Vertext구조의 멤버변수 순서 같아야함..
	D3D11_INPUT_ELEMENT_DESC layout[] = {
		{"POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0},
	};

	pDevice->CreateInputLayout(layout, ARRAYSIZE(layout),
							vertextShaderBuffer->GetBufferPointer(), 
							vertextShaderBuffer->GetBufferSize(), 
							&vertexInputLayout);

	pDeviceContext->IASetInputLayout(vertexInputLayout);

	
	// 기본 토폴로지(위상) 설정.(TRIANGLELIST로 설정) -> 점으로 뭘할건지 정해줘야함.
	// 삼가곃은 list 아니면 srip을 많이씀 
	// list는 각각 포인트 3개씩 다필요함 
	// strip은 정점을 공유할수있음 선긋는방식이다르다.
	// 결국 텍스쳐를 맵핑할려면 각 삼각형당 정점이 다 필요하니까 list해야함
	pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// 뷰포트 설정 - 9버전은 기본 뷰포트가있는데 여기는엇ㅂㅇ븜
	// 영역설정
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
