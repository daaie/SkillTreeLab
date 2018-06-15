#include <iostream>
#include <stack>
#include <stdlib.h>
#include <string>

using namespace std;

int ReverseString()
{
	stack<char> Storage;
	char Buffer[1024] = { 0 };
	string Buffer2;
	//char* Buffer2 = new char[1024];

	//scanf("%s", Buffer);
	cin >> Buffer2;

	//for (int i = 0; i < strlen(Buffer); ++i)
	//{
	//	Storage.push(Buffer[i]);
	//}
	//for (int i = 0; i < strlen(Buffer); ++i)
	//{
	//	printf("%c", Storage.top());
	//	Storage.pop();
	//}
	//printf("\n");

	for (int i = 0; i < (int)Buffer2.length(); ++i)
	{
		Storage.push(Buffer2[i]);
	}

	for (int i = 0; i < (int)Buffer2.length(); ++i)
	{
		printf("%c", Storage.top());
		Storage.pop();
	}
	printf("\n");


	return 0;
}

int ExprTest()
{
	stack<char> Par;
	string Expr;
	cin >> Expr;

	for (int i = 0; i < Expr.length(); ++i)
	{
		char temp = Expr[i];
		if (temp == '(' || temp == '[' || temp == '{')
		{
			Par.push(temp);
		}
		else if (temp == ')' || temp == ']' || temp == '}')
		{
			if (Par.top() == '(' || Par.top() == '[' || Par.top() == '{')
			{
				Par.pop();
			}
		}
	}

	if (Par.empty())
	{
		cout << "OK" << endl;
	}
	else
	{
		cout << "Wrong" << endl;
	}

	return 0;
}

int ToBinary()
{
	stack<int> Binary;

	int number = 383737;

	while (number > 1)
	{
		int result = number / 2;
		int mod = number % 2;

		Binary.push(mod);

		number = result;
	}
	Binary.push(number);

	while (!Binary.empty())
	{
		cout << Binary.top();
		Binary.pop();
	}

	cout << endl;

	return 0;
}

int main()
{
	return 0;
}