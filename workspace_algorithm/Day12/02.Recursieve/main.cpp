//재귀함수 자기호출함수
//반복문 <-> 재귀호출
//트리탐색 전위탐색 중위탐색 후위탐색
#include <iostream>
#include <string>

using namespace std;
/*
void func(int i)
{
	if (i == 0)
		return; 

	cout << "hello" << endl;
	func(i-1);
}

int main()
{
	func(5);

	return 0;
}

*/
int normalAdd(int n)
{
	int sum = 0;
	for (int i = 1; i <= n; i++)
		sum += i;
	
	return sum;
}

int recursiveAdd(int n)
{
	//1. 끝나는 식이 꼭 필요하다..
	if (n == 1)
		return n;

	//2. 자기자신을 호출한다.
	return n + recursiveAdd(n - 1);
}

int recursiveArraySum(int *data, int n)
{
	if (n == 1)
		return data[n-1];

	return data[n-1]+ recursiveArraySum(data,n-1);
}

int normalArraySum(int *data, int n)
{
	
	int sum = 0;
	for (int i = 0; i < n; i++)
		sum += data[i];

	return sum;
}

void normalReversePrint(string str)
{
	for (int i = str.length()-1; i >=0; i--)
	{
		//cout << str[i - 1];
		cout << str.substr(i,1); //i번째 배열부터 1자리 까지, 1안주면 끝까지 자름, 
	}
	cout << endl;
}

void recursiveReversePrint(string str)
{
	if (str.length() == 0)
		return;
	
	recursiveReversePrint(str.substr(1));

	cout << str[0];
}

int normalStringLength(string str)
{
	int i = 0;
	while (str[i] != '\0')
	{
		i++;
	}
	return i;
}

int recursiveStringLength(string str)
{
	if (str[0] == '\0')
		return 0;

	return 1 + recursiveStringLength(str.substr(1));
}

/*
unsigned long long binary = 0;
int i = 1;

while (n > 0)
{
int mod = n % 2;
n = n / 2;
binary += i * mod;
i *= 10;
}

cout << binary << endl;
*/

void normalBinaryPrint(int n)
{
	char buffer[2];
	string binary;

	while (n != 0)
	{
		memset(buffer, 0x00, sizeof(buffer));
		itoa(n % 2, buffer, 10);
		binary += buffer;
		n = n / 2;
	}
	
	normalReversePrint(binary);
}

void recursiveBinaryPrint(int n)
{
	if (n == 0)
		return;

	recursiveBinaryPrint(n / 2);
	cout << n % 2;
}

int normalFactorial(int n)
{
	int sum = 1;
	for (int i = 1; i <= n; i++)
	{
		sum *= i;
	}
	return sum;
}

int recursiveFactorial(int n)
{
	if (n == 1)
		return n;

	return n*recursiveFactorial(n - 1);
}

int normalSearch(int *data, int n, int target)
{
	for (int i = 0; i < n; i++)
	{
		if (data[i] == target)
			return i;
	}
	return -1;
}

int recursiveSearch(int *data, int n, int target)
{
	//없는 값 넣으면 에러남..ㅋㅋㅋ
	if (n < 0)
		return -1;
	if (data[n-1] == target)
		return n-1;

	return recursiveSearch(data , n-1 ,target);
}
int rs2 = 0;
int recursiveSearch2(int *data, int begin, int end, int target)
{
	rs2++;
	//못찾았으면 -1
	if (begin > end)
		return -1;
	else if (data[begin] == target)
		return begin;
	else
	{
		return recursiveSearch2(data, begin + 1, end, target);
	}
}
//책 12장 분할 정복법?
int rs3 = 0;
int recursiveSearch3(int *data, int begin, int end, int target)
{
	rs3++;
	if (begin > end)
	{
		return -1;
	}
	else
	{
		int middle = (begin + end) / 2;
		if (data[middle] == target)
			return middle;
		//앞에꺼 찾기
		int index = recursiveSearch3(data, begin, middle - 1, target);
		if (index != -1)
			return index;
		else
			//뒤에꺼 찾기
			return recursiveSearch3(data, middle + 1, end, target);
	}
}
int bs = 0;
int binarySearch(int *data, int begin, int end, int target)
{
	bs++;
	if (begin > end)
	{
		return -1;
	}

	int middle = (begin + end) / 2;
	if (data[middle] == target)
	{
		return middle;
	}
	else if (data[middle] > target)
	{
		return binarySearch(data, begin, middle - 1, target);
	}
	else if(data[middle] < target)
	{
		return binarySearch(data, middle + 1, end, target);
	}
}
int main()
{
	int data[] = { 1,2,3,4,5,6,7,8,9,10 };
	//data가 정렬되어있으면 binary search ,, 계속계속 반을 버릴수있다.

	cout << normalAdd(10) << endl;
	cout << recursiveAdd(10) << endl;

	cout << normalArraySum(data, 10) << endl;
	cout << recursiveArraySum(data, 10) << endl;

	string str = "Hello";

	cout << normalStringLength(str) << endl;
	cout << recursiveStringLength(str) << endl;

	normalReversePrint(str);
	recursiveReversePrint(str);
	cout << endl;

	normalBinaryPrint(7);
	recursiveBinaryPrint(7);
	cout << endl;

	cout << normalFactorial(3) << endl;
	cout << recursiveFactorial(3) << endl;

	cout << normalSearch(data,10,1) << endl;
	cout << recursiveSearch(data, 10,4) << endl;
	//------------------------------------------------------
	cout << recursiveSearch2(data,0, 9, 3) << endl;
	cout << recursiveSearch3(data, 0, 9, 3) << endl;
	cout << binarySearch(data, 0, 9, 3) << endl;

	cout << bs << endl;
	cout << rs3 << endl;
	cout << rs2 << endl;
	return 0;
}