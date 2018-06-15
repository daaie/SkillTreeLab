#include <stdlib.h>
#include <stdio.h>
#include "DLList.h"
#include <list>

//using namespace std;

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



int main()
{
	//DLList<int> IntList;

	//IntList.AddNode(1);
	//IntList.AddNode(2);
	//IntList.AddNode(3);

	//IntList.InsertAfter(2, 10);
	//IntList.InsertAfter(2, 11);
	//IntList.InsertBefore(3, 20);

	//IntList.DeleteNode(1);

	//DLList<float> FloatList;

	//FloatList.AddNode(1.0f);
	//FloatList.AddNode(22.f);
	//FloatList.AddNode(33.343f);

	//FloatList.InsertAfter(1.0f, 10.2f);
	//FloatList.InsertAfter(1.0f, 11.f);
	//FloatList.InsertBefore(22.f, 20.1f);

	//FloatList.DeleteNode(22.f);

	//for (DLLNode<int>* Current = IntList.Begin(); Current != IntList.End()->Next; Current = Current->Next)
	//{
	//	printf("%d\n", Current->Value);
	//}

	std::list<int> l;

	l.push_back(1);
	l.push_back(2);
	l.push_back(3);
	l.push_back(4);

	for (std::list<int>::iterator i = l.begin(); i != l.end(); i++)
	{
		printf("%d\n", *i);
	}

	return 0;
}