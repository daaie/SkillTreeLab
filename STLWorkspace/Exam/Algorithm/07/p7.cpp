#include "DLList.h"

template<typename T>
class LStack
{
protected:
	DLList<T> dList;

public:
	T Pop(); 
	T Peek(); 
	void Push(T Value); 
	int Length() { return dList.Length(); };

};

template<typename T>
T LStack<T>::Pop()
{
	T Value = 0;
	if (Length() > 0)
	{
		Value = dList.End()->Value;
		dList.DeleteNode(Value);
	}
	return Value;
}

template<typename T>
T LStack<T>::Peek()
{
	T Value = 0;
	if (Length() > 0)
	{
		Value = dList.End()->Value;
	}
	return Value;
}

template<typename T>
void LStack<T>::Push(T Value)
{
	dList.AddNode(Value);
}

int main()
{
	LStack<int> lStack;

	for (int i = 1; i <= 10 ; i++)
	{
		lStack.Push(i);
	}

	for (int i = 1; i <= 10; i++)
	{
		cout<<lStack.Pop()<<endl;
	}

	return 0;
}