#include "Engine.h"

//DX�� �׻� ������ ��������� release

int WINAPI WinMain(
	HINSTANCE hinstance, 
	HINSTANCE previnstance, // �Ⱦ� ȣȯ��
	LPSTR lpCmdLine, //�Ķ���� ����
	int nCmdLine) //define�Ǿ��ִ� �Ķ������..
{
	Engine engine(hinstance);

	if (engine.Init() == false)
		return 1;

	return engine.Run();
}
