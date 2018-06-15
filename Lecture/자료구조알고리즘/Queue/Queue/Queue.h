#pragma once
#include <list>

using namespace std;

template<typename T>
class Queue
{
public:
	void Enqueue(T Value);
	void Dequeue(T& Value);
	bool IsEmpty();
	int Length();
	void Peek(T& Value);

protected:
	list<T> Data;
};

template<typename T>
void Queue<T>::Enqueue(T Value)
{
	Data.push_back(Value);
}

template<typename T>
void Queue<T>::Dequeue(T & Value)
{
	Value = Data.front();
	Data.pop_front();
}

template<typename T>
bool Queue<T>::IsEmpty()
{
	return (Data.empty());
}

template<typename T>
int Queue<T>::Length()
{
	return Data.size();
}

template<typename T>
void Queue<T>::Peek(T & Value)
{
	Value = Data.front();
}
