#include <Windows.h>	// 윈도우 생성하기 위해 필요.
#include "Engine.h"

int WINAPI WinMain(
	HINSTANCE hinstnace, 
	HINSTANCE previnstance,
	LPSTR lpCmdLine, 
	int nCmdLine)
{

	Engine engine(hinstnace);

	if (engine.Init() == false) 
		return 0;

	return engine.Run();
}