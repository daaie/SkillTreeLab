#include <stdlib.h>
#include "SList.h"

Node* NewNode(int Value)
{
	Node* New = (Node*)malloc(sizeof(Node));
	New->Value = Value;
	New->Next = NULL;

	return New;
}


Node* Search(Node** PointerHead, int Value)
{
	for (Node* Current = *PointerHead; Current != NULL; Current = Current->Next)
	{
		if (Current->Value == Value)
		{
			return Current;
		}
	}

	return NULL;
}

Node* Search(Node** PointerHead, Node* SearchNode)
{
	for (Node* Current = *PointerHead; Current != NULL; Current = Current->Next)
	{
		if (Current == SearchNode)
		{
			return Current;
		}
	}

	return NULL;
}

void AddLastNew(Node** PointerHead, int i)
{
	Node* New = NewNode(i);

	if (*PointerHead == NULL)
	{
		*PointerHead = New;
	}
	else
	{
		Node* Last = *PointerHead;
		while (Last->Next != NULL)
		{
			Last = Last->Next;
		}
		Last->Next = New;
	}
}

void AddFirstNew(Node** PointerHead, int i)
{
	Node* New = NewNode(i);

	if (*PointerHead == NULL)
	{
		*PointerHead = New;
	}
	else
	{
		New->Next = *PointerHead;
		*PointerHead = New;
	}
}

void AddAfterSearch(Node** PointerHead, int SearchValue, int NewValue)
{
	bool Add = false;

	Node* FindNode = Search(PointerHead, SearchValue);
	if (FindNode)
	{
		//찾은거 뒤에 추가
		Node* New = NewNode(NewValue);
		New->Next = FindNode->Next;
		FindNode->Next = New;
	}
	else //검색이 안되는 값이면 뒤에 추가
	{
		AddLastNew(PointerHead, NewValue);
	}
}

void DeleteNode(Node** PointerHead, int SearchValue)
{
	Node* PrevNode = NULL;
	for (Node* Current = *PointerHead; Current != NULL; Current = Current->Next)
	{
		if (Current->Value == SearchValue)
		{
			if (PrevNode == NULL)
			{
				Node* NewHead = Current->Next;
				free(Current);
				*PointerHead = NewHead;
			}
			else
			{
				PrevNode->Next = Current->Next;
				free(Current);
			}
			return;
		}

		PrevNode = Current;
	}
}

void Destory(Node** PointerHead)
{
	Node* Current = *PointerHead;

	while (Current != NULL)
	{
		Node* Temp = Current->Next;
		free(Current);
		Current = Temp;
	}
}


