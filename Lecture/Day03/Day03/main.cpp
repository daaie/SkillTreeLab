#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

#include "SList.h"
#include "test.h"




int main()
{
	//Node* NewNode = (Node*)malloc(sizeof(Node));
	//NewNode->Value = 1;
	//NewNode->Next = NULL;

	//Node* NewNode2 = (Node*)malloc(sizeof(Node));
	//NewNode2->Value = 2;
	//NewNode2->Next = NULL;
	//NewNode->Next = NewNode2;

	//Node* NewNode3 = (Node*)malloc(sizeof(Node));
	//NewNode3->Value = 3;
	//NewNode3->Next = NULL;
	//NewNode2->Next = NewNode3;



	Node* Head = NULL;
	for (int i = 1; i <= 10; ++i)
	{
		if (i % 2 == 0)
		{
			AddFirstNew(&Head, i);
		}
		else
		{
			AddLastNew(&Head, i);
		}
	}

	AddAfterSearch(&Head, 200, 100);

	DeleteNode(&Head, 3);
	DeleteNode(&Head, 5);
	DeleteNode(&Head, 100);
	DeleteNode(&Head, 10);

	for (Node* Current = Head; Current != NULL; Current = Current->Next)
	{
		printf("%d\n", Current->Value);
	}

	Destory(&Head);





	return 0;
}