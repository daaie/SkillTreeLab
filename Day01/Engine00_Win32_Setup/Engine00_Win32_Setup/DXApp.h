#pragma once
#include <Windows.h>

class DXApp
{
public:
	DXApp(HINSTANCE hinstance);
	virtual ~DXApp(); //가상소멸자, 소멸자는 가끔 자식이 안사라짐..가상소멸자가 안전.

	// Main Message Loop .
	int Run();
	
	// 프레임워크 메소드.
	virtual bool Init(); 
	virtual void Update() = 0; // 구현강제 방식=자식에서 반드시 구현=순수가상함수.
	virtual void Render() = 0; // 순수가상함수를 포함하는 함수는 추상클래스가됨 . 
	// 추상 클래스가 되면 인스턴스를 만들 수 없게 됩니다. 포인터가능.
	// 반드시 상속, 모든 순수 멤버 가상함수를 오버라이드하면
	// 해당 자식 클래스부터 인스턴스를 다시 생성할 수 있습니다.
	// 추상 클래스는 ‘추상적인 형태’만 제안하고, 실제 구현은 자식 클래스로 미루기 위한 용도입니다.

	virtual LRESULT MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam); 
	// 메세지 프로시저. LRESULT CALLBACK 함수명();
	// 윈도우는 프로그램이랑 콜백방식 메세지를 주고받는데, 메세지 처리하는 함수.
	// 윈도우에서 프로그램에 메세지 전달할때 요 함수 통함.

protected:
	// Win32 멤버 변수.
	HWND		hwnd; //타입모르면 f12타고 가ㅅㅓ 봐..
	HINSTANCE	hinstance;
	UINT		clientWidth;
	UINT		clientHeight;
	LPCSTR		appTitle; // LPCWSTR을 써야할수도있따.(유니코드) , LPCSTR = 멀티바이트
	DWORD		wndStyle; // unsigned long -> double이라고 보면됨...

protected:
	// Win32 메소드.
	// 윈도우 초기화.
	bool InitWindow();
};

