#include <stdio.h>
#include <iostream>
#include <stack>
#include <string>

using namespace std;

int main()
{
	string eq;

	printf("¼ö½Ä : ");
	cin >> eq;

	stack<char> a;

	printf("%d\n", eq.length());

	for (int i = 0; i < (int)eq.length(); i++)
	{
		char temp = eq[i];
	
		if(temp =='('|| temp == '[' || temp == '{' )
		{
			a.push(eq[i]);
		}
		else if (temp == ')' || temp == '}' || temp == ']')
		{
			a.pop();
		}
	}

	if (a.empty())
		printf("OK\n");
	else
		printf("ERROR\n");

	return 0;
}