#include <iostream>
#include <stack>

using namespace std;

int binarySearch(int *data, int begin, int end, int target)
{
	if (begin > end) return -1; 

	int middle = (begin + end) / 2;
	
	if (data[middle] == target)
	{
		return middle;
	}
	else if (data[middle] < target)
	{
		binarySearch(data,middle+1,end,target);
	}
	else
	{
		binarySearch(data, begin, middle - 1, target);
	}

}

int main()
{
	int data[] = {1,2,3,4,5,6,7,8,9,10};
	int number = -1;
	
	while(number != 0)
	{
		cout << "찾을 수(나가려면 0) : ";
		cin >> number;
		cout << "index : " << binarySearch(data, 0, 9, number) << endl;
	}
	return 0;
}