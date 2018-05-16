#pragma once
#include <Windows.h>

class DXApp
{
public:
	DXApp(HINSTANCE hinstance);
	virtual ~DXApp(); //����Ҹ���, �Ҹ��ڴ� ���� �ڽ��� �Ȼ����..����Ҹ��ڰ� ����.

	// Main Message Loop .
	int Run();
	
	// �����ӿ�ũ �޼ҵ�.
	virtual bool Init(); 
	virtual void Update() = 0; // �������� ���=�ڽĿ��� �ݵ�� ����=���������Լ�.
	virtual void Render() = 0; // ���������Լ��� �����ϴ� �Լ��� �߻�Ŭ�������� . 
	// �߻� Ŭ������ �Ǹ� �ν��Ͻ��� ���� �� ���� �˴ϴ�. �����Ͱ���.
	// �ݵ�� ���, ��� ���� ��� �����Լ��� �������̵��ϸ�
	// �ش� �ڽ� Ŭ�������� �ν��Ͻ��� �ٽ� ������ �� �ֽ��ϴ�.
	// �߻� Ŭ������ ���߻����� ���¡��� �����ϰ�, ���� ������ �ڽ� Ŭ������ �̷�� ���� �뵵�Դϴ�.

	virtual LRESULT MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam); 
	// �޼��� ���ν���. LRESULT CALLBACK �Լ���();
	// ������� ���α׷��̶� �ݹ��� �޼����� �ְ�޴µ�, �޼��� ó���ϴ� �Լ�.
	// �����쿡�� ���α׷��� �޼��� �����Ҷ� �� �Լ� ����.

protected:
	// Win32 ��� ����.
	HWND		hwnd; //Ÿ�Ը𸣸� f12Ÿ�� ������ ��..
	HINSTANCE	hinstance;
	UINT		clientWidth;
	UINT		clientHeight;
	LPCSTR		appTitle; // LPCWSTR�� ����Ҽ����ֵ�.(�����ڵ�) , LPCSTR = ��Ƽ����Ʈ
	DWORD		wndStyle; // unsigned long -> double�̶�� �����...

protected:
	// Win32 �޼ҵ�.
	// ������ �ʱ�ȭ.
	bool InitWindow();
};

