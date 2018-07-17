#pragma once

#include <d3d11.h>
#include <d3dx11.h>
#include <fbxsdk.h>
#include <dinput.h>
#include <xnamath.h>
#include <vector>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dx11.lib")
#pragma comment(lib, "libfbxsdk.lib")

#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "dinput8.lib")

// 메모리 해제용 도움 함수(helper function).
namespace Memory
{
	// DX 인터페이스 해제용.
	template <class T> void SafeRelease(T& t)
	{
		if (t)
		{
			t->Release();
			t = nullptr;
		}
	}

	// 동적 할당한 인스턴스 해제용.
	template <class T> void SafeDelete(T& t)
	{
		if (t)
		{
			delete t;
			t = nullptr;
		}
	}

	// 배열 해제용.
	template <class T> void SafeDeleteArr(T& t)
	{
		if (t)
		{
			delete[] t;
			t = nullptr;
		}
	}
}