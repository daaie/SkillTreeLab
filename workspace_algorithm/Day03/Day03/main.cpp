#include <stdlib.h>
#include "SList.h"

/*
int main()
{
	int *a = (int *)malloc(sizeof(int) * 4);

	a[0] = 1;
	a[1] = 2;
	a[2] = 3;
	a[3] = 4;

	for (int i = 0; i < 4; i++)
	{
		printf("%d\n", a[i]);
	}

	//a -> 10°³

	int *b = (int*)malloc(sizeof(int) * 10);
	memset(b, 0x00, sizeof(int) * 10);
	
	//memcpy(b, a, sizeof(int) * 4);
	for(int i=0;i<10;i++)
	{
		b[i]=a[i];
	}

	free(a);
	a = b;
	
	for (int i = 0; i < 10; i++)
	{
		printf("%d\n", a[i]);
	}
	
	return 0;
}
*/

/*
void addNode(Node *firstNode, int value)
{
	Node *node = firstNode;
	while (node->Next != NULL)
	{
		node = node->Next;
	}

	Node *newNode = (Node *)malloc(sizeof(Node));
	newNode->value = value;
	newNode->Next = NULL;
	node->Next=newNode;

}

int main()
{
	Node *newNode = (Node *)malloc(sizeof(Node));
	newNode->value= 1;
	newNode->Next = NULL;
	
	addNode(newNode, 2);
	addNode(newNode, 3);

	return 0;
}
*/

int main()
{
	Node *Head = NULL;
	for (int i = 1; i <= 10; i++)
	{
		if (i % 2 == 0)
		{
			AddLastNode(&Head,i);
		}
		else
		{
			AddHead(&Head,i);
		}
	}

	AddAfter(&Head,100,3);
	AddBefore(&Head, 200, 3);
	DelNode(&Head,9);
	DelNode(&Head,3);

	PrintNode(Head);

	DestroyNode(&Head);

	return 0;
}

