#include <stdio.h>

void swap(int *a, int *b)
{
	int temp = *a;
	*a = *b;
	*b = temp;
}

int partition(int *data, int begin, int end)
{
	int pivot = data[end];
	int min = begin - 1;

	for (int max = begin; max <= end - 1; max++)
	{
		if (data[max] <= pivot)
		{
			min++;
			swap(&data[min], &data[max]);
		}
	}
	swap(&data[min + 1], &data[end]);

	return min + 1;
}

void quickSort(int *data, int left, int right)
{
	if (left<right)
	{
		int index = partition(data, left, right);

		quickSort(data, left, index - 1);
		quickSort(data, index + 1, right);
	}
}
int main()
{
	int data[] = { 9,2,3,7,1,4,8,10,6,5 };
	int i = 0;

	printf("정렬 전 : ");
	for (i = 0; i<10; i++)
	{
		printf("%d\t", data[i]);
	}
	printf("\n");

	quickSort(data, 0, 9);

	printf("정렬 후 : ");
	for (i = 0; i<10; i++)
	{
		printf("%d\t", data[i]);
	}
	printf("\n");

	return 0;
}