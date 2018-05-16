#include <Windows.h> //윈도우를 생성하기 위해 필요.
#include "Engine.h"

int WINAPI WinMain(
	HINSTANCE hinstance,//프로그램인스턴스
	HINSTANCE previnstance, // 안씀 호환용
	LPSTR lpCmdLine, //파라미터 라인
	int nCmdLine) //define되어있는 파라미터임..
{
	//테스트용 메시지 박스 , NULL 자리에 핸들 들어가면됨
	//MB_OK 에서 F12 -> 데피니션으로감
	//MessageBox(NULL,"테스트 창입니다", "테스트", MB_YESNO);

	//engine d인스턴스 생성
	Engine engine(hinstance);

	if (engine.Init() == false)
		return 0;

	//엔진 실행
	return engine.Run();
}
