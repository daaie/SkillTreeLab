#include <Windows.h>	// 윈도우 생성하기 위해 필요.
#include "Engine.h"

int WINAPI WinMain(
	HINSTANCE hinstnace, 
	HINSTANCE previnstance,
	LPSTR lpCmdLine, 
	int nCmdLine)
{
	// 테스트용. 메시지 박스.
	//MessageBox(NULL, L"테스트 창입니다.", L"테스트", MB_YESNO);

	// Engine 인스턴스 생성.
	Engine engine(hinstnace);
	// 초기화.
	if (engine.Init() == false) 
		return 0;

	// 엔진 실행.
	return engine.Run();
}