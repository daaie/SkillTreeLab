#include <Windows.h>
#include "Engine.h"

int WINAPI WinMain(
	HINSTANCE hinstnace, 
	HINSTANCE previnstance,
	LPSTR lpCmdLine, 
	int nCmdLine)
{
	// Engine �ν��Ͻ� ����.
	Engine engine(hinstnace);

	// �ʱ�ȭ.
	if (engine.Init() == false) 
		return 0;

	// ���� ����.
	return engine.Run();
}