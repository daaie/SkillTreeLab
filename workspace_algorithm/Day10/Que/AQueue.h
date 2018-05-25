#pragma once
//#include <Queue>
#include <list>

using namespace std;

template<typename T>
class Aqueue
{
protected:
	T* data;
	int head;
	int tail;
	int size;
	int capacity;

public:
	void init(int size = 10);
	void enqueue(T value);
	void dequeue(T& value);
	void peek(T& value);

	bool isEmpty() { return size == 0; }
	int length() { return size; }
};

template<typename T>
inline void Aqueue<T>::init(int value)
{
	data = new T[value];
	capacity = value;
	head = 0;
	tail = 0;
	size = 0;
}

template<typename T>
inline void Aqueue<T>::enqueue(T value)
{
	if (size < capacity)
	{
		data[tail++] = value;
		tail = tail%capacity;
		size++;
	}
}

template<typename T>
inline void Aqueue<T>::dequeue(T & value)
{
	if(size>0)
	{
		value = data[head];
		data[head] = -1;
		head++;
		head = head%capacity;
		size--;
	}
}

template<typename T>
inline void Aqueue<T>::peek(T & value)
{
	value = data[head];
}
