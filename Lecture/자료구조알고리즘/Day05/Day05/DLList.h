#pragma once

#include <stdio.h>
#include <stdlib.h>

template<typename T>
class DLLNode
{
public:
	T Value;
	DLLNode* Prev;
	DLLNode* Next;
};


template<typename T>
class DLList
{
private:
	DLLNode<T>* Head;
	DLLNode<T>* Tail;

	DLLNode<T>* MakeNode(T Value);

public:
	DLList();

	void AddNode(T Value);
	void DeleteNode(T Value);
	void InsertBefore(T SearchValue, T Value);
	void InsertAfter(T SearchValue, T Value);

	DLLNode<T>* Begin() { return Head->Next; }
	DLLNode<T>* End() { return Tail->Prev; }
};

template<typename T>
DLList<T>::DLList()
{
	Head = NULL;
	Tail = NULL;
}

template<typename T>
DLLNode<T>* DLList<T>::MakeNode(T Value)
{
	//DLLNode<T>* Temp = (DLLNode<T>*)malloc(sizeof(DLLNode<T>));
	DLLNode<T>* Temp = new DLLNode<T>();
	Temp->Value = Value;
	Temp->Prev = NULL;
	Temp->Next = NULL;

	return Temp;
}

template<typename T>
void DLList<T>::AddNode(T Value)
{
	if (Head == NULL)
	{
		Head = MakeNode(NULL);
		Tail = MakeNode(NULL);
		Head->Next = Tail;
		Tail->Prev = Head;

		DLLNode<T>* NewNode = MakeNode(Value);

		Head->Next = NewNode;
		NewNode->Prev = Head;
		Tail->Prev = NewNode;
		NewNode->Next = Tail;
	}
	else
	{
		DLLNode<T>* NewNode = MakeNode(Value);

		Tail->Prev->Next = NewNode; // 마지막꺼의 다음꺼는 새로 생긴거
		NewNode->Next = Tail;		//새로 생긴에 다음꺼는 꼬리
		NewNode->Prev = Tail->Prev; //새로 생긴거에 앞에꺼는 꼬리에 전꺼(마지막꺼)
		Tail->Prev = NewNode;	//꼬리전꺼는 새로 생긴거		
	}
}

template<typename T>
void DLList<T>::DeleteNode(T Value)
{
	for (DLLNode<T>* Current = Head->Next; Current != Tail; Current = Current->Next)
	{
		if (Current->Value == Value)
		{
			Current->Prev->Next = Current->Next;
			Current->Next->Prev = Current->Prev;
			delete(Current);

			return;
		}
	}
}

template<typename T>
void DLList<T>::InsertBefore(T SearchValue, T Value)
{
	for (DLLNode<T>* Current = Head->Next; Current != Tail; Current = Current->Next)
	{
		if (Current->Value == SearchValue)
		{
			DLLNode<T>* NewNode = MakeNode(Value);
			Current->Prev->Next = NewNode;
			NewNode->Prev = Current->Prev;
			NewNode->Next = Current;
			Current->Prev = NewNode;
		}
	}
}

template<typename T>
void DLList<T>::InsertAfter(T SearchValue, T Value)
{
	for (DLLNode<T>* Current = Head->Next; Current != Tail; Current = Current->Next)
	{
		if (Current->Value == SearchValue)
		{
			DLLNode<T>* NewNode = MakeNode(Value);

			Current->Next->Prev = NewNode;
			NewNode->Next = Current->Next;
			NewNode->Prev = Current;
			Current->Next = NewNode;
		}
	}
}