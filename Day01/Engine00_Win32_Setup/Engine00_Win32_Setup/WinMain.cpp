#include <Windows.h> //�����츦 �����ϱ� ���� �ʿ�.
#include "Engine.h"

int WINAPI WinMain(
	HINSTANCE hinstance,//���α׷��ν��Ͻ�
	HINSTANCE previnstance, // �Ⱦ� ȣȯ��
	LPSTR lpCmdLine, //�Ķ���� ����
	int nCmdLine) //define�Ǿ��ִ� �Ķ������..
{
	//�׽�Ʈ�� �޽��� �ڽ� , NULL �ڸ��� �ڵ� �����
	//MB_OK ���� F12 -> ���Ǵϼ����ΰ�
	//MessageBox(NULL,"�׽�Ʈ â�Դϴ�", "�׽�Ʈ", MB_YESNO);

	//engine d�ν��Ͻ� ����
	Engine engine(hinstance);

	if (engine.Init() == false)
		return 0;

	//���� ����
	return engine.Run();
}
