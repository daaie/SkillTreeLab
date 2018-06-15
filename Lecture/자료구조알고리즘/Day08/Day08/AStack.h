#pragma once
#include <string.h>

template<typename T>
class AStack
{
protected:
	T* Data;
	int Capacity;
	int Size;
	int Top;

public:
	void Init(int _Size = 10);
	T Pop();
	void Push(T Value);
	int Length();
	T Peek();
};

template<typename T>
void AStack<T>::Init(int _Size = 10)
{
	Data = new T[_Size];
	Capacity = _Size;
	Size = 0;
	Top = -1;
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
void AStack<T>::Push(T Value)
{
	if (Capacity > Size)
	{
		Top++;
		Data[Top] = Value;
		Size++;
	}
	else
	{
		//저장공간 늘리기
		Capacity = Capacity * 2;
		T* NewData = new T[Capacity];
		memcpy(NewData, Data, (sizeof(T) * Size));
		delete[] Data;
		Data = NewData;

		//자료 집어 넣기
		Top++;
		Data[Top] = Value;
		Size++;
	}

}

template<typename T>
int AStack<T>::Length()
{
	return Size;
}

template<typename T>
T AStack<T>::Peek()
{
	return Data[Top];
}