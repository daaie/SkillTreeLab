#include "Engine.h"

//DX는 항상 해제를 잘해줘야함 release

int WINAPI WinMain(
	HINSTANCE hinstance, 
	HINSTANCE previnstance, // 안씀 호환용
	LPSTR lpCmdLine, //파라미터 라인
	int nCmdLine) //define되어있는 파라미터임..
{
	Engine engine(hinstance);

	if (engine.Init() == false)
		return 1;

	return engine.Run();
}
