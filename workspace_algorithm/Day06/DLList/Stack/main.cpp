#include "stdio.h"
#include "stdlib.h"
#include "LStack.h"

using namespace pdaios;

class AStack
{
protected:
	int *Data;
	int Capacity;
	int Size;
	int Top;

public:
	void Init(int _Size);
	void Push(int Value);
	int Pop();
	int Peek();
	int Length();
};

void AStack::Init(int _Size = 10)
{
	Data = new int[_Size];
	Capacity = _Size;
	printf("Capacity1 : %d \n", Capacity);
	Size = 0;
	Top = -1;
}

void AStack::Push(int Value)
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
		int *NewData = new int[Capacity];
		printf("Capacity2 : %d \n", Capacity);
		memcpy(NewData, Data, sizeof(int) * Size);

		delete[] Data; // 배열일때는 delete[]로 지워야함
		Data = NewData;

		Top++;
		Data[Top] = Value;
		Size++;
	}
}

int AStack::Pop()
{
	int Value = 0;
	if (Size > 0)
	{
		Value = Data[Top];
		Top--;
		Size--;
	}

	return Value;
}

int AStack::Peek()
{
	return Data[Top];
}

int AStack::Length()
{
	return Size;
}


int main()
{
	/*LStack
	LStack<int> a;

	a.Push(1);
	a.Push(2);
	a.Push(3);
	a.Push(4);
	a.Push(5);
	a.Push(6);
	a.Push(7);
	a.Push(8);

	int b = a.Pop();
	printf("pop : %d\n", b);
	b=a.Pop();
	printf("pop : %d\n", b);

	while (a.Length() > 0)
	{
		printf("%d\n", a.Pop());
	}

	LStack<char> c;
	for (int i = 'Z'; i >= 'A'; --i)
	{
		c.Push(i);
	}

	printf("peek : %c , lenght : %d \n", c.Peek(), c.Length());

	while (c.Length() > 0)
	{
		printf("%c\n", c.Pop());
	}
	*/
	AStack a;
	a.Init();
	for (int i = 100; i >= 1; --i)
	{
		a.Push(i);
	}

	int c;

	do
	{
		c = a.Pop();
		printf("%d\n", c);

	} while (a.Length() != 0);

	return 0;
}
