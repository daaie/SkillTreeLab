#pragma once

#include<stdio.h>
#include<stdlib.h>

template<typename T>
class DLNode
{
public:
	T Value;
	DLNode* Prev;
	DLNode* Next;
};

template<typename T>
class DLList
{
private:
	DLNode<T> *Head;
	DLNode<T> *Tail;
	DLNode<T>* MakeNode(T Value);
	DLNode<T>* SearchNode(T Value);

public:
	DLList();
	void AddNode(T Value);
	void AddNode(T Value, T SearchValue);
	void AddBefore(T Value, T SearchValue);
	void DeleteNode(T Value);

	inline DLNode<T> *Begin() { return Head->Next; }
	DLNode<T> *End() { return Tail->Prev; }
};	

template<typename T>
DLList<T>::DLList()
{
	Head = NULL;
	Tail = NULL;
};

template<typename T>
DLNode<T>* DLList<T>::MakeNode(T Value)
{
	DLNode<T>* Temp = (DLNode<T>*)malloc(sizeof(DLNode<T>));
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

		DLNode<T>* NewNode = MakeNode(Value);

		Head->Next = NewNode;
		NewNode->Prev = Head;
		Tail->Prev = NewNode;
		NewNode->Next = Tail;
	}
	else
	{
		DLNode<T>* NewNode = MakeNode(Value);

		Tail->Prev->Next = NewNode; // 마지막꺼의 다음꺼는 새로 생긴거
		NewNode->Next = Tail;		//새로 생긴에 다음꺼는 꼬리
		NewNode->Prev = Tail->Prev; //새로 생긴거에 앞에꺼는 꼬리에 전꺼(마지막꺼)
		Tail->Prev = NewNode;	//꼬리전꺼는 새로 생긴거		
	}
}

template<typename T>
DLNode<T>* DLList<T>::SearchNode(T Value)
{
	for (DLNode<T>* Current = Head->Next; Current != Tail; Current = Current->Next)
	{
		if (Current->Value == Value)
		{
			return Current;
		}
	}
	return NULL;
}

template<typename T>
void DLList<T>::AddNode(T Value, T SearchValue)
{
	if (Head == NULL)
	{
		Head = MakeNode(0);
		Tail = MakeNode(0);
		Head->Next = Tail;
		Tail->Prev = Head;

		DLNode<T>* NewNode = MakeNode(Value);

		Head->Next = NewNode;
		NewNode->Prev = Head;
		Tail->Prev = NewNode;
		NewNode->Next = Tail;
	}
	else
	{
		DLNode<T>* TempNode = SearchNode(SearchValue);
		if (TempNode == NULL) return;

		DLNode<T>* NewNode = MakeNode(Value);

		TempNode->Next->Prev = NewNode;
		NewNode->Prev = TempNode;
		NewNode->Next = TempNode->Next;
		TempNode->Next = NewNode;
	}
}

template<typename T>
void DLList<T>::AddBefore(T Value, T SearchValue)
{
	if (Head == NULL)
	{
		Head = MakeNode(0);
		Tail = MakeNode(0);
		Head->Next = Tail;
		Tail->Prev = Head;

		DLNode<T>* NewNode = MakeNode(Value);

		Head->Next = NewNode;
		NewNode->Prev = Head;
		Tail->Prev = NewNode;
		NewNode->Next = Tail;
	}
	else
	{
		DLNode<T>* TempNode = SearchNode(SearchValue);
		if (TempNode == NULL) return;

		DLNode<T>* NewNode = MakeNode(Value);

		TempNode->Prev->Next = NewNode;
		NewNode->Prev = TempNode->Prev;
		NewNode->Next = TempNode;
		TempNode->Prev = NewNode;

	}
}

template<typename T>
void DLList<T>::DeleteNode(T Value)
{
	for (DLNode<T>* Current = Head->Next; Current != Tail; Current = Current->Next)
	{
		if (Current->Value == Value)
		{
			Current->Prev->Next = Current->Next;
			Current->Next->Prev = Current->Prev;
			free(Current);
			return;
		}
	}
}