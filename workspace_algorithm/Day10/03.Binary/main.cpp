#include <stdio.h>
#include "AStack.h"

using namespace pdaios;

int main()
{
	int n;
	printf("¼ýÀÚ : ");
	scanf("%d", &n);
	
	AStack<int> a;
	a.Init(32);

	int temp;

	while (n!=0)
	{
		temp = n % 2;		
		a.Push(temp);
		n = n / 2;

	}

	for (int i = a.Length(); i > 0; i--)
	{
		printf("%d",a.Pop());
	}

	printf("\n");

	return 0;
}