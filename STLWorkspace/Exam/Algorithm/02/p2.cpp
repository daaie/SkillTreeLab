#include <stdio.h>

int main()
{
	int arr[100] = { 0 };

	for (int i = 0; i < 100; i++)
	{
		arr[i] = i+1;
		printf("%d\t", *(arr+i));
	}

	return 0;
}
