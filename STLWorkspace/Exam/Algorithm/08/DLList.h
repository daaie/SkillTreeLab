#pragma once

#include <iostream>

using namespace std;

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

	int Cnt;

public:
	DLList();
	void AddNode(T Value);
	void AddNode(T Value, T SearchValue);
	void AddBefore(T Value, T SearchValue);
	void DeleteNode(T Value);

	int Length() { return Cnt; }

	DLNode<T>* IndexOf(int Index);
	inline DLNode<T> *Begin() { return Head->Next; }
	DLNode<T> *End() { return Tail->Prev; }
};

template<typename T>
DLList<T>::DLList()
{
	Head = NULL;
	Tail = NULL;

	Cnt = 0;
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

		Tail->Prev->Next = NewNode; 
		NewNode->Next = Tail;
		NewNode->Prev = Tail->Prev; 
		Tail->Prev = NewNode;			
	}
	Cnt++;
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
	Cnt++;
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
	Cnt++;
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
	Cnt--;
}

template<typename T>
DLNode<T>* DLList<T>::IndexOf(int Index)
{
	int ObjectCount = 0;
	for (DLNode<T>* Current = Head; Current != Tail; Current = Current->Next)
	{
		if ((ObjectCount == Index) && (Index != 0) && Index <= Cnt)
		{
			return Current;
		}
		ObjectCount++;
	}

	return NULL;
}