#pragma once

#include <d3d11.h>
#include <d3dx11.h>

#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"d3dx11.lib")

// 메모리 해제용 도움 함수(helper function).
namespace Memory
{
	// DX 인터페이스 헤제용
	template<class T> void SafeRelease(T& T)
	{
		if (T)
		{
			T->Release();
			T = nullptr;
		}
	}

	// 동적할당한 인스턴스 해제용.
	template<class T> void SafeDelete(T& T)
	{
		if (T)
		{
			delete T;
			T = nullptr;
		}
	}

	// 배열 해제용
	template<class T> void SafeDeleteArr(T& T)
	{
		if (T)
		{
			delete[] T;
			T = nullptr;
		}
	}
}