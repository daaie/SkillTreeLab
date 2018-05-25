#include <stdlib.h>
#include <stdio.h>


//struct Node
//{
//	int Value;
//	Node* Next;
//};
//
//Node* Head;
//
//
//int main()
//{
//	Head = (Node*)malloc(sizeof(Node));
//	Head->Value = 1;
//	Head->Next = NULL;
//
//	Node* Next1 = (Node*)malloc(sizeof(Node));
//	Next1->Value = 2;
//	Next1->Next = NULL;
//
//	Head->Next = Next1;
//
//	Node* Next2 = (Node*)malloc(sizeof(Node));
//	Next2->Value = 3;
//	Next2->Next = NULL;
//
//	Next1->Next = Next2;
//
//
//	for (Node* Current = Head; Current != NULL; Current = Current->Next)
//	{
//		printf("%d\n", Current->Value);
//	}
//
//	Head->Next = Next2;
//	free(Next1);
//
//	for (Node* Current = Head; Current != NULL; Current = Current->Next)
//	{
//		printf("%d\n", Current->Value);
//	}
//
//
//	return 0;
//}

typedef struct tagNode
{
	int Value;
	tagNode* Prev;
	tagNode* Next;
} Node;

Node* Head = NULL;
Node* Tail = NULL;


Node* MakeNode(int Value)
{
	Node* Temp = (Node*)malloc(sizeof(Node));
	Temp->Value = Value;
	Temp->Prev = NULL;
	Temp->Next = NULL;

	return Temp;
}

void AddNode(int Value)
{
	if (Head == NULL)
	{
		Head = MakeNode(0);
		Tail = MakeNode(0);
		Head->Next = Tail;
		Tail->Prev = Head;

		Node* NewNode = MakeNode(Value);

		Head->Next = NewNode;
		NewNode->Prev = Head;
		Tail->Prev = NewNode;
		NewNode->Next = Tail;
	}
	else
	{
		Node* NewNode = MakeNode(Value);

		Tail->Prev->Next = NewNode; // 마지막꺼의 다음꺼는 새로 생긴거
		NewNode->Next = Tail;		//새로 생긴에 다음꺼는 꼬리
		NewNode->Prev = Tail->Prev; //새로 생긴거에 앞에꺼는 꼬리에 전꺼(마지막꺼)
		Tail->Prev = NewNode;	//꼬리전꺼는 새로 생긴거		
	}
}

Node* SearchNode(int Value)
{
	for (Node* Current = Head->Next; Current != Tail; Current = Current->Next)
	{
		if (Current->Value == Value)
		{
			return Current;
		}
	}
	return NULL;
}

void AddNode(int Value, int SearchValue)
{
	if (Head == NULL)
	{
		Head = MakeNode(0);
		Tail = MakeNode(0);
		Head->Next = Tail;
		Tail->Prev = Head;

		Node* NewNode = MakeNode(Value);

		Head->Next = NewNode;
		NewNode->Prev = Head;
		Tail->Prev = NewNode;
		NewNode->Next = Tail;
	}
	else
	{
		Node* TempNode = SearchNode(SearchValue);
		if (TempNode == NULL) return;

		Node* NewNode = MakeNode(Value);
		
		TempNode->Next->Prev = NewNode;
		NewNode->Prev = TempNode;
		NewNode->Next = TempNode->Next;
		TempNode->Next = NewNode;
	}
}

void AddBefore(int Value, int SearchValue)
{
	if (Head == NULL)
	{
		Head = MakeNode(0);
		Tail = MakeNode(0);
		Head->Next = Tail;
		Tail->Prev = Head;

		Node* NewNode = MakeNode(Value);

		Head->Next = NewNode;
		NewNode->Prev = Head;
		Tail->Prev = NewNode;
		NewNode->Next = Tail;
	}
	else
	{
		Node* TempNode = SearchNode(SearchValue);
		if (TempNode == NULL) return;

		Node* NewNode = MakeNode(Value);

		TempNode->Prev->Next = NewNode;
		NewNode->Prev = TempNode->Prev;
		NewNode->Next = TempNode;
		TempNode->Prev = NewNode;

	}
}
void DeleteNode(int Value)
{
	for (Node* Current = Head->Next; Current != Tail; Current = Current->Next)
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

int main()
{

	for (int i = 1; i <= 10; i++)
	{
		AddNode(i);
	}

	for (Node* Current = Head->Next; Current != Tail; Current = Current->Next)
	{
		printf("%d\n", Current->Value);
	}

	AddNode(100, 3);

	AddBefore(200,2);

	for (Node* Current = Head->Next; Current != Tail; Current = Current->Next)
	{
		printf("%d\n", Current->Value);
	}
	DeleteNode(2);

	for (Node* Current = Tail->Prev; Current != Head; Current = Current->Prev)
	{
		printf("%d\n", Current->Value);
	}




	//
	//Head = (Node*)malloc(sizeof(Node));
	//Head->Value = 1;
	//Head->Prev = NULL;
	//Head->Next = NULL;

	//Node* Next1 = (Node*)malloc(sizeof(Node)); //1
	//Next1->Value = 2;
	//Next1->Prev = NULL;
	//Next1->Next = NULL;

	//Head->Next = Next1;							//2
	//Next1->Prev = Head;							//3

	//Node* Next2 = (Node*)malloc(sizeof(Node)); //1
	//Next2->Value = 3;
	//Next2->Prev = NULL;
	//Next2->Next = NULL;

	//Next1->Next = Next2;							//2
	//Next2->Prev = Next1;							//3

	//Tail = Next2;

	return 0;
}