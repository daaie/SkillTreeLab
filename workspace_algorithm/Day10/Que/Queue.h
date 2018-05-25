#pragma once
//#include <Queue>
#include <list>

using namespace std;

template<typename T>
class queue
{
protected:
	list<T> Data;

public:
	void enqueue(T value);
	void dequeue(T& value);
	void peek(T& value);

	bool isEmpty();
	int length();
};

template<typename T>
void queue<T>::enqueue(T value)
{
	Data.push_back(value);
}

template<typename T>
void queue<T>::dequeue(T& value)
{
	value = Data.front();
	Data.pop_front();
}

template<typename T>
void queue<T>::peek(T & value)
{
	value = Data.front();
}

template<typename T>
bool queue<T>::isEmpty()
{
	return Data.empty();
}

template<typename T>
int queue<T>::length()
{
	return Data.size();
}
