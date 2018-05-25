#include <iostream>

using namespace std;

//qsort함수도 있음..

void printData(int *data, int n)
{
	for (int i = 0; i < n; i++)
		cout << data[i] << " " ;
	cout << endl;
}

int selectionSort()
{
	int data[10] = { 6,5,1,2,8,9,10,3,4,7 };

	printData(data, 10);

	for (int i = 0; i < 10; i++)
	{
		for (int j = i + 1; j < 10; j++)
		{
			if (data[i] > data[j])
			{
				int temp = data[j];
				data[j] = data[i];
				data[i] = temp;
			}
		}
		printData(data, 10);
	}
	return 0;
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
			swap(data[min], data[max]);
		}
	}
	swap(data[min + 1], data[end]);

	return min+1;
}

void quickSort(int *data, int begin, int end)
{
	if (begin < end)
	{
		printData(data, 10);
		int pivot = partition(data, begin, end);
		quickSort(data,begin,pivot-1);
		quickSort(data, pivot+1,end);
	}
}

void bubblesort(int *data, int n)
{
	for(int i=0;i<n-1;i++)
	{
		for (int j = 0; j < n-1; j++)
		{
			if (data[j] > data[j + 1])
			{
				swap(data[j], data[j + 1]);
			}
		}
	}
}

int main()
{
	int data[10] = { 6,5,1,2,8,9,10,3,4,7 };
	//quickSort(data,0,9);
	//printData(data,10);

	bubblesort(data, 10);
	printData(data, 10);
	return 0;
}