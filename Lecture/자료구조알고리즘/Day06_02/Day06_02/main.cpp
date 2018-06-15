#include "stdio.h"
#include "stdlib.h"
#include "AStack.h"




int main()
{
	AStack<int> a;
	a.Init();

	for (int i = 100; i >= 1; --i)
	{
		a.Push(i);
	}

	while (a.Length() != 0)
	{
		int c = a.Pop();
		printf("%d\n", c);
	} 

	return 0;
}
