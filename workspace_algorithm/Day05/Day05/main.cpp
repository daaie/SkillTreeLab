#include <stdlib.h>
#include "SLList.h"

int main()
{
	Node *Head = NewNode(1);

	AddLastNew(Head, 2);
	AddLastNew(Head, 2);
	AddLastNew(Head, 3);
	AddLastNew(Head, 4);
	PrintNode(Head);

	DeleteNode(&Head, 3);
	PrintNode(Head);

	DeleteNode(&Head, 1);
	PrintNode(Head);

	DeleteNode(&Head, 2);
	PrintNode(Head);

	return 0;
}