#pragma once

#include <d3d11.h>
#include <d3dx11.h>

#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"d3dx11.lib")

// �޸� ������ ���� �Լ�(helper function).
namespace Memory
{
	// DX �������̽� ������
	template<class T> void SafeRelease(T& T)
	{
		if (T)
		{
			T->Release();
			T = nullptr;
		}
	}

	// �����Ҵ��� �ν��Ͻ� ������.
	template<class T> void SafeDelete(T& T)
	{
		if (T)
		{
			delete T;
			T = nullptr;
		}
	}

	// �迭 ������
	template<class T> void SafeDeleteArr(T& T)
	{
		if (T)
		{
			delete[] T;
			T = nullptr;
		}
	}
}