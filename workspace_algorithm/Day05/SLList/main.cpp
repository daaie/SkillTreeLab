#include <stdio.h>
#include <stdlib.h>

typedef struct tagNode
{
	int Value;
	tagNode *Prev;
	tagNode *Next;
}Node;

Node * Head = NULL;
Node * Tail = NULL;

Node* MakeNode(int Value)
{
	Node *Temp = (Node *)malloc(sizeof(Node));
	Temp->Value = Value;
	Temp->Prev = NULL;
	Temp->Next = NULL;

	return Temp;
}

void AddNode(int Value)
{
	if (Head == NULL)
	{
		Head = MakeNode(Value);
		Tail = Head;
	}
	else
	{
		Node *Temp = MakeNode(Value);

		Tail->Next = Temp;
		Temp->Prev = Tail;
		Tail = Temp;
	}
}

void DeleteNode(int Value)
{
	Node *Prev = NULL;
	Node *tmp = NULL;
	for (Node *Current = Head; Current != NULL; Current = tmp)
	{
		tmp = Current->Next;
		if (Current->Value == Value)
		{
			if (Current != Head)
			{
				Current->Prev->Next = tmp;
				if (tmp != NULL)
				{
					Current->Next->Prev = Current->Prev;
				}
				else
				{
					Tail = Current->Prev;
				}
			}
			else
			{
				Head = Current->Next;
				Head->Prev = NULL;
			}
			free(Current);
			continue;
		}
	}
}
int main()
{
	//Head = (Node *)malloc(sizeof(Node));
	//Head->Value = 1;
	//Head->Next = NULL;
	//Head->Prev = NULL;

	//Node *Node1 = (Node *)malloc(sizeof(Node));
	//Node1->Value = 2;
	//Node1->Prev = NULL;
	//Node1->Next = NULL;

	//Head->Next = Node1;
	//Node1->Prev = Head;

	//Node *Node2 = (Node *)malloc(sizeof(Node));
	//Node2->Value = 3;
	//Node2->Prev = NULL;
	//Node2->Next = NULL;

	//Node1->Next = Node2;
	//Node2->Prev = Node1;

	//Tail = Node2;
	AddNode(1);
	AddNode(2);
	AddNode(3);

	for (Node *Current = Head; Current != NULL; Current = Current->Next)
	{
		printf("%d\n", Current->Value);
	}

	DeleteNode(3);
	for (Node *Current = Tail; Current != NULL; Current = Current->Prev)
	{
		printf("%d\n", Current->Value);
	}
	return 0;
}