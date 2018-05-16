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
	swapDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapDesc.BufferDesc.RefreshRate.Numerator = 60; 
	swapDesc.BufferDesc.RefreshRate.Denominator = 1; 

	swapDesc.SampleDesc.Count = 1; 
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

	// 장치및 스왑체인설정 
	hr=D3D11CreateDeviceAndSwapChain(NULL,D3D_DRIVER_TYPE_HARDWARE, 
								NULL, NULL, NULL, NULL,D3D11_SDK_VERSION,
								&swapDesc,&pSwapChain,&pDevice,NULL,&pDeviceContext); 
	
	// if (SUCCEEDED(hr)) // 성공여부 S_OK // 실패여부 E_FAIL
	if (FAILED(hr)) 
	{
		MessageBox(NULL, "장치생성 실패","오류",MB_OK);
		return false;
	}

	// 2D 텍스쳐 생성(렌더 타겟 용)
	// Texutre2D는 ID311Resource (지오메트리, 텍스쳐, 쉐이더) 를 상속..
	ID3D11Texture2D* pBackBufferTexture;

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
	pDeviceContext->OMSetRenderTargets(1, &pRenderTargetView,NULL);

	// 3. 리소스 해제..리소스를 pBackBufferTexture애가 물고있으니까
	Memory::SafeRelease(pBackBufferTexture);


	return true;
}

bool DXApp::InitScene()
{  
	HRESULT hr;

	hr = D3DX11CompileFromFile("Effects.fx", NULL, NULL, "VS_Main", "vs_5_0", NULL, NULL, NULL, &vertextShaderBuffer, NULL, NULL);
	if (FAILED(hr))
	{
		MessageBox(NULL, "D3DX11CompileFromFile111 Fail", "오류", MB_OK);
		return false;
	}

	// vertextShaderBuffer는 쉐이더 ㅓ컴파일된 바이트코드 -> VS 오브젝트생성 -> 파이프라인에 바인딩

	// 정점 셰이더 오브젝트 생성
	pDevice->CreateVertexShader(vertextShaderBuffer->GetBufferPointer(),
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

	// uv값이 정점에서만 정의함.
	// 실제 픽셀 쉐이더에 
	// 레스터 라이저 단계에서 픽셀로 만들기 위한 값을 보간 
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
	// 정점 버퍼 생성 ( 버퍼 desc, 서브 리소스. )
	D3D11_BUFFER_DESC vbDesc;
	ZeroMemory(&vbDesc, sizeof(D3D11_BUFFER_DESC));

	vbDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbDesc.ByteWidth = sizeof(Vertex) * ARRAYSIZE(vertices);
	vbDesc.CPUAccessFlags = 0; 
	vbDesc.MiscFlags = 0;
	vbDesc.Usage = D3D11_USAGE_DEFAULT; 

	// 정점 버퍼 서브 리소스 생성 - vertex 정보
	D3D11_SUBRESOURCE_DATA vbData;
	ZeroMemory(&vbData, sizeof(D3D11_SUBRESOURCE_DATA));
	vbData.pSysMem = vertices;

	// 정점 buffer 생성 // 버퍼 리소스 vertexBuffer는 서브리소스들의 집합으로 볼수있다.
	pDevice->CreateBuffer(&vbDesc, &vbData, &vertexBuffer);


	// 인덱스 버퍼 생성 ( 버퍼 desc, 서브 리소스. )
	D3D11_BUFFER_DESC ibDesc;
	ZeroMemory(&ibDesc, sizeof(D3D11_BUFFER_DESC));
	ibDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibDesc.ByteWidth = sizeof(DWORD) * ARRAYSIZE(indices);
	ibDesc.CPUAccessFlags = 0;
	ibDesc.MiscFlags = 0;
	ibDesc.Usage = D3D11_USAGE_DEFAULT;

	// 인덱스 버퍼 서브 리소스 생성 - vertex 정보
	D3D11_SUBRESOURCE_DATA ibData;
	ZeroMemory(&ibData, sizeof(D3D11_SUBRESOURCE_DATA));
	ibData.pSysMem = indices;

	// 인덱스버퍼 생성
	pDevice->CreateBuffer(&ibDesc, &ibData, &indexBuffer);


	// 정점 버퍼 바인딩
	UINT stride = sizeof(Vertex);
	UINT offset = 0;

	// 버텍스정보(지오메트리정보) 인풋어셈블러(IA)한테 알려줘야한다 - 파이프라인에 물리는과정
	pDeviceContext->IASetVertexBuffers(0,1,&vertexBuffer, &stride, &offset);

	// 인덱스 버퍼 바인딩
	pDeviceContext->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);


	D3D11_INPUT_ELEMENT_DESC layout[] = {
		{ "POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 },
		{ "TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,12,D3D11_INPUT_PER_VERTEX_DATA,0 }, 
		//TEXTURE가 여러개면 TEXCOORD0 ,1,2,3, 숫자붙이면된다. 쉐이더 에서만... 레이아웃에서는 붙이면 에러난다.
	};

	pDevice->CreateInputLayout(layout, ARRAYSIZE(layout),
							vertextShaderBuffer->GetBufferPointer(), 
							vertextShaderBuffer->GetBufferSize(), 
							&vertexInputLayout);

	pDeviceContext->IASetInputLayout(vertexInputLayout);

	pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// 영역설정
	D3D11_VIEWPORT viewport;
	ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));
	viewport.TopLeftX =0;
	viewport.TopLeftY = 0;
	viewport.Width = clientWidth;
	viewport.Height = clientHeight;

	pDeviceContext->RSSetViewports(1, &viewport);

	// world * view * position
	// 월드 행렬. (물체 위치/회전/스케일)
	world = XMMatrixIdentity(); //단위 행렬 만들어주는 함수
	// 지속적으로 카메라가 회전하는 프로그램의 경우 Update 함수에서 world 변경시켜주면 된다.
	// 여기서는 뭐 카메라가 변하지않네..
	
	// 카메라 위치
	cameraPos = XMVectorSet(3.0f, 4.0f, -7.0f, 0.0f);

	// 카메라가 바라보는 위치
	cameraTarget = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);

	// 카메라가 위 방향 ( 방향만 중요하므로 숫자는 상관없다)
	cameraUp = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	// 뷰 행렬 LH는 왼손좌표계
	view = XMMatrixLookAtLH(cameraPos, cameraTarget, cameraUp);

	// 투영 행렬
	// 세로 시야각 - 세로만 맞추고 가로는 종횡비에따라 맞춤 
	float fovY = 75.0f * 3.14 / 180.0f;	// 라디안으로 각도 넘김

	// 카메라 설정 - orthographic(2D처럼..), perspective(원근법) 투영방식 ..
	projection = XMMatrixPerspectiveFovLH(fovY,
			(float)clientWidth/clientHeight, 1.0f, 1000.0f);

	// 상수 버퍼 데이터 설정 및 파이프라인 바인딩/
	// 전치 행렬을 만들어서 데이터 채우기
	// DX는 행우선 , HLSL은 열우선.
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


	// 서브 리소스
	D3D11_SUBRESOURCE_DATA cbData;
	ZeroMemory(&cbData, sizeof(D3D11_SUBRESOURCE_DATA));
	cbData.pSysMem = &data;

	// 상수 버퍼 생성
	pDevice->CreateBuffer(&cbDesc, &cbData, &cbBuffer);

	// 파이프라인에 바인딩
	pDeviceContext->VSSetConstantBuffers(0, 1, &cbBuffer); // 넘겨줄 쉐이더 잘 선택해서 넘겨줘라

	//텍스쳐 로드 초기화
	if (!InitTexture())
		return false;
	
	//텍스쳐 픽셀 쉐이더 파이프 라인에 연결
	pDeviceContext->PSSetShaderResources(0, 1, &pTexture);

	// 샘플러 스테이트 바인딩
	pDeviceContext->PSSetSamplers(0, 1, &pTextureSamplerState);

	//dx에서 조명같은거는 최소한으로하고 쉐이더에서 처리함 쉐이더를 잘짜서 넘겨줌
	return true;
}

bool DXApp::InitTexture()
{
	HRESULT hr;
	// 텍스쳐 파일 로드
	hr = D3DX11CreateShaderResourceViewFromFile(pDevice,"dx.png",NULL,NULL,&pTexture,NULL);
	if (FAILED(hr))
	{
		MessageBox(NULL,"D3DX11CreateShaderResourceViewFromFile111", "오류 ",MB_OK);
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

	// 칼라값을 어떻게 읽을지 서술자로 서술 -> 파이프라인에
	hr = pDevice->CreateSamplerState(&samplerDesc, &pTextureSamplerState);
	if (FAILED(hr))
	{
		MessageBox(NULL, "CreateSamplerState", "오류 ", MB_OK);
		return false;
	}

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
