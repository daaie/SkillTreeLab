#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <stack>
#include <string>

//반복문 <-> 재귀호출 //트리 탐색, 전위 탐색, 중위 탐색, 후위 탐색
using namespace std;

int NormalSum(int n)
{
	int sum = 0;
	for (int i = 1; i <= n; ++i)
	{
		sum += i;
	}

	return sum;
}

//재귀함수로 1부터 n까지 합 만들기
int RecursiveSum(int n)
{
	if (n == 1)
		return 1;

	return n + RecursiveSum(n - 1);
}

int NormalArraySum(int* data, int n)
{
	int sum = 0;
	for (int i = 0; i < n; ++i)
	{
		sum += data[i];
	}

	return sum;
}

int RecursiveArraySum(int* data, int n)
{
	if (n == 0)
		return 0;

	return data[n-1] + RecursiveArraySum(data, n-1);
}

void NormalReversePrint(string str)
{
	for (int i = str.length()-1; i >= 0; --i)
	{
		cout << str.substr(i, 1);
	}

	cout << endl;
}

void RecursiveReversePrint(string str)
{
	if (str.length() == 0)
		return;


	RecursiveReversePrint(str.substr(1));
	cout << str[0];
}

int NomralStringLength(string str)
{
	int i = 0;
	for(int i = 0; ; ++i)
	{
		if (str[i] == '\0')
		{
			return i;
		}
	}
}


int RecursiveStringLength(string str)
{
	if (str[0] == '\0')
		return 0;

	return 1 + RecursiveStringLength(str.substr(1));
}

//2진수 출력
void NormalBinaryPrint(int n)
{
	string temp;
	char buffer[1024];

	while ( n >= 2)
	{
		int mod = n % 2;
		n = n / 2;
		sprintf(buffer, "%d", mod);
		temp = temp + buffer;
	}

	if (n != 0)
	{
		sprintf(buffer, "%d", n);
		temp = temp + buffer;
	}

	RecursiveReversePrint(temp);
}

void RecursiveBinaryPrint(int n)
{
	if (n < 2)
	{
		cout << n;
	}
	else
	{
		RecursiveBinaryPrint(n / 2);
		cout << (n % 2);
	}
}

//팩토리얼 계산 3!

int NormalFactorial(int n)
{
	int sum = 1;
	for (int i = 1; i <= n; ++i)
	{
		sum *= i;
	}

	return sum;
}

int RecursiveFactorial(int n)
{
	if (n == 1)
		return 1;

	return n * RecursiveFactorial(n - 1);
}

int NormalSearch(int* data, int n, int target)
{
	for (int i = 0; i < n; i++)
	{
		if (data[i] == target)
		{
			return i;
		}
	}

	return -1;
}

int SearchTime = 0;

int RecursiveSearch(int* data, int n, int target)
{
	if (n < 0)
		return -1;

	if (data[n] == target)
		return n;

	return RecursiveSearch(data, n-1, target);
}


int RecursiveSearch2(int* data, int begin, int end, int target)
{
	SearchTime++;
	if (begin > end)
	{
		return -1;
	}
	else if (data[begin] == target)
	{
		return begin;
	}
	else
	{
		RecursiveSearch2(data, begin + 1, end, target);
	}
}

int RecursiveSearch3(int* data, int begin, int end, int target)
{
	if (begin > end)
	{
		return -1;
	}
	else 
	{
		int middle = (begin + end) / 2;
		if (data[middle] == target)
		{
			return middle;
		}
		//앞에꺼 찾기 
		int index = RecursiveSearch3(data, begin, middle - 1, target);
		if (index != -1)
		{
			return index;
		}
		else
		{
			return RecursiveSearch3(data, middle + 1, end, target);
		}
	}
}


int BSearch(int* data, int begin, int end, int target)
{
	SearchTime++;
	if (begin > end)
	{
		return -1;
	}
	else
	{
		int middle = (begin + end) / 2;
		if (data[middle] == target)
		{
			return middle;
		}
		else if (data[middle] > target)
		{
			return BSearch(data, begin, middle - 1, target);
		}
		else// if(data[middle] > target)
		{
			return BSearch(data, middle + 1, end, target);
		}
	}

}

//배열 탐색


int main()
{

	int data[10] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };

	cout << RecursiveSearch2(data, 0, 9, 10) << endl;

	return 0;
}

