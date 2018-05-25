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
		void Init(int _Size = 10); //이렇게 선언도 해줘야 한다.힝..
		void Push(T Value);
		T Pop();
		T Peek();
		int Length();
	};

	template<typename T>
	//tempate 는 만들 떄 디폴트 값을 못먹는다...힝 .. 선언에도 해줘야함..
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
		if (Capacity > Size) //배열넘어가는지 확인!!
		{
			Top++;
			Data[Top] = Value;
			Size++;
		}
		else
		{
			Capacity++; // 요거보다는 Capacity = Capacity * 2; 가 낫다...
						//시간을 쓰냐 공간을 쓰냐
			T *NewData = new T[Capacity];
			//printf("Capacity2 : %d \n", Capacity);
			memcpy(NewData, Data, sizeof(T) * Size);

			delete[] Data; // 배열일때는 delete[]로 지워야함
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