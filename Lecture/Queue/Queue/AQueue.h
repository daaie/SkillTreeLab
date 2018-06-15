#pragma once
template<typename T>
class AQueue
{
public:
	void Init(int size = 3);
	void Enqueue(T Value);
	void Dequeue(T& Value);
	bool IsEmpty() { return Size == 0; }
	int Length() { return Size; }
	void Peek(T& Value);

protected:
	T* Data;
	int Head;
	int Tail;
	int Size;
	int Capacity;
};



template<typename T>
void AQueue<T>::Init(int size = 3)
{
	Data = new T[size];
	Capacity = size;
	Head = 0;
	Tail = 0;
	Size = 0;
}

template<typename T>
void AQueue<T>::Enqueue(T Value)
{
	if (Size < Capacity)
	{
		Data[Tail++] = Value;

		Tail = Tail % Capacity;
		Size++;
	}
}

template<typename T>
void AQueue<T>::Dequeue(T & Value)
{
	if (Size > 0)
	{
		Value = Data[Head];
		Data[Head] = -1;
		Head++;
		Head = Head % Capacity;
		Size--;
	}
}

template<typename T>
void AQueue<T>::Peek(T & Value)
{
	Value = Data[Head];
}
