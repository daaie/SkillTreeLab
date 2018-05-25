#include <stdio.h>
#include <stdlib.h>
#include "SLList.h"

Node *NewNode(int Value)
{
	Node *NewNode = (Node*)malloc(sizeof(Node));
	NewNode->Value = Value;
	NewNode->Next = NULL;

	return NewNode;
}

void AddLastNew(Node* PHead, int Value)
{
	Node *SearchNode = PHead;
	
	while (SearchNode->Next != NULL)
	{
		SearchNode = SearchNode->Next;
	}
	SearchNode->Next = NewNode(Value);
}
void DeleteNode(Node** PHead, int Value)
{
	Node *Prev;
	Node *tmp;
	for (Node *SearchNode = *PHead; SearchNode != NULL; SearchNode = tmp)
	{
		tmp = SearchNode->Next;
		if (SearchNode->Value == Value)
		{
			if (SearchNode != *PHead)
			{
				Prev->Next = SearchNode->Next;			
				free(SearchNode);
			}
			else
			{
				free(SearchNode);
				*PHead = tmp;
			}
			continue;
		}
		Prev = SearchNode;
	}
}

void PrintNode(Node *PHead)
{
	Node *SearchNode = PHead;
	while (SearchNode != NULL)
	{
		printf("%d\n", SearchNode->Value);
		printf("-------------------\n");
		SearchNode = SearchNode->Next;
	}
}

