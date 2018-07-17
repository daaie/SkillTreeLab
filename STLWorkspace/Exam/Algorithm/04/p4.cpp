#include "SLList.h"

int main()
{
	SLList<int> slist;
	
	for (int i = 1; i <= 10; i++)
	{
		slist.AddLastNew(i);
	}
	
	slist.PrintNode();

	return 0;
}
