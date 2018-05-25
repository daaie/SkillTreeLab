#pragma once

#include <stdio.h>
#include "DLList.h"

using namespace pdaios;

namespace pdaios
{
	template<typename T>
	class AStack
	{
	protected:
		T *Data;
		int Capacity;
		int Size;
		int Top;

	public:
		void Init(int _Size = 10); //�̷��� ���� ����� �Ѵ�.��..
		void Push(T Value);
		T Pop();
		T Peek();
		int Length();
	};

	template<typename T>
	//tempate �� ���� �� ����Ʈ ���� ���Դ´�...�� .. ���𿡵� �������..
	void AStack<T>::Init(int _Size = 10)
	{
		Data = new T[_Size];
		Capacity = _Size;
		//printf("Capacity1 : %d \n", Capacity);
		Size = 0;
		Top = -1;
	}

	template<typename T>
	void AStack<T>::Push(T Value)
	{
		if (Capacity > Size) //�迭�Ѿ���� Ȯ��!!
		{
			Top++;
			Data[Top] = Value;
			Size++;
		}
		else
		{
			Capacity++; // ��ź��ٴ� Capacity = Capacity * 2; �� ����...
						//�ð��� ���� ������ ����
			T *NewData = new T[Capacity];
			//printf("Capacity2 : %d \n", Capacity);
			memcpy(NewData, Data, sizeof(T) * Size);

			delete[] Data; // �迭�϶��� delete[]�� ��������
			Data = NewData;

			Top++;
			Data[Top] = Value;
			Size++;
		}
	}

	template<typename T>
	T AStack<T>::Pop()
	{
		T Value = 0;
		if (Size > 0)
		{
			Value = Data[Top];
			Top--;
			Size--;
		}

		return Value;
	}

	template<typename T>
	T AStack<T>::Peek()
	{
		return Data[Top];
	}

	template<typename T>
	int AStack<T>::Length()
	{
		return Size;
	}
}