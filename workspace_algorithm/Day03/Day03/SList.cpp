#include <stdio.h>
#include <stdlib.h>
#include "SList.h"

Node *NewNode(int Value)
{
	Node *New = (Node *)malloc(sizeof(Node));
	New->Value = Value;
	New->Next = NULL;

	return New;
}

void PrintNode(Node *head)
{
	Node *node = head;
	while (node != NULL)
	{
		printf("%d\n", node->Value);
		node = node->Next;
	}
}
void AddLastNode(Node **head, int i)
{
	Node *newNode = NewNode(i);

	if (*head == NULL)
	{
		*head = newNode;
	}
	else
	{
		Node *node = *head;
		while (node->Next != NULL)
		{
			node = node->Next;
		}

		node->Next = newNode;
	}
}

void AddHead(Node** head, int i)
{
	Node *newNode = NewNode(i);

	if (*head != NULL)
	{
		newNode->Next = *head;
		*head = newNode;
	}
	else
	{
		*head = newNode;
	}
}

void AddAfter(Node **head, int i, int value)
{
	bool Add = 0;
	Node *newNode = NewNode(i);

	Node *node = *head;
	while (node != NULL)
	{
		if (node->Value == value)
		{
			newNode->Next = node->Next;
			node->Next = newNode;
			Add = 1;
		}
		node = node->Next;
	}

	if (!Add)
	{
		AddLastNode(head, i);
	}
}

void AddBefore(Node **head, int i, int value)
{
	bool Add = 0;

	Node *newNode = NewNode(i);

	Node *node = *head;
	Node *prev = NULL;

	while (node != NULL)
	{
		if (node->Value == value)
		{
			if (node != *head)
			{
				newNode->Next = node;
				prev->Next = newNode;
			}
			else
			{
				AddHead(head, i);
			}
		}
		prev = node;
		node = node->Next;
	}
}

void DelNode(Node **head, int value)
{
	Node *prev = NULL;

	for (Node *Current = *head; Current != NULL; Current = Current->Next)
	{
		if (Current->Value == value)
		{
			if (prev != NULL)
			{
				prev->Next = Current->Next;
				free(Current);
				Current = NULL;
			}
			else
			{
				Node *NewHead = Current->Next;
				free(Current);
				*head = NewHead;
			}
			break;
		}
		prev = Current;
	}
}

void DestroyNode(Node **Head)
{
	Node *Current = *Head;

	while (Current != NULL)
	{
		Node *tmp = Current->Next;
		free(Current);
		Current = tmp;
	}
}
