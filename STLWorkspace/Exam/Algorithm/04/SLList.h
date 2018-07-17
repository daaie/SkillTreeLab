#pragma once
#include <iostream>

using namespace std;

template<typename T>
class SLLNode
{
public:
	T Value;
	SLLNode *Next;
};

template<typename T>
class SLList
{
private:
	SLLNode<T>* Head;
	SLLNode<T> *NewNode(T Value);
public:
	SLList();
	void AddLastNew(T Value);
	void PrintNode();
};

template<typename T>
SLList<T>::SLList()
{
	Head = NewNode(0);
}

template<typename T>
SLLNode<T>* SLList<T>::NewNode(T Value)
{
	SLLNode<T> *NewNode = (SLLNode<T>*)malloc(sizeof(SLLNode<T>));
	NewNode->Value = Value;
	NewNode->Next = NULL;

	return NewNode;
}

template<typename T>
void SLList<T>::AddLastNew(T Value)
{
	SLLNode<T> *SearchNode = Head;

	while (SearchNode->Next != NULL)
	{
		SearchNode = SearchNode->Next;
	}
	SearchNode->Next = NewNode(Value);
}

template<typename T>
void SLList<T>::PrintNode()
{
	SLLNode<T>* SearchNode = Head->Next;
	while (SearchNode != NULL)
	{
		cout << SearchNode->Value << "\t";
		SearchNode = SearchNode->Next;
	}
}