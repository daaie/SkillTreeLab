#include "DLList.h"

template<typename T>
class LQueue
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
T LQueue<T>::Pop()
{
	T Value = 0;
	if (Length() > 0)
	{
		Value = dList.Begin()->Value;
		dList.DeleteNode(Value);
	}
	return Value;
}

template<typename T>
T LQueue<T>::Peek()
{
	T Value = 0;
	if (Length() > 0)
	{
		Value = dList.Begin()->Value;
	}
	return Value;
}

template<typename T>
void LQueue<T>::Push(T Value)
{
	dList.AddNode(Value);
}

int main()
{
	LQueue<int> lQueue;

	for (int i = 1; i <= 10 ; i++)
	{
		lQueue.Push(i);
	}

	for (int i = 1; i <= 10; i++)
	{
		cout<<lQueue.Pop()<<endl;
	}

	return 0;
}