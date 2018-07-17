#include <iostream>
#include <stack>

using namespace std;

char oper[] = { "()+-*/" };
int weight[] = { 1,1,2,2,3,3 };

int main()
{
	char equ[1024] = { 0 };
	cout << "╫д ют╥б : ";
	cin >> equ;

	stack<char> st;

	int offset = 0;
	bool operflag = false;
	while (equ[offset] != '\0')
	{
		operflag = false;

		for (int i = 0; i < strlen(oper); i++)
		{
			if (equ[offset] == oper[i])
			{
				cout << " ";
				if (st.size() <= 0)
				{
					st.push(equ[offset]);
				}
				else
				{
					for (int j = 0; j < strlen(oper); j++)
					{
						if (st.top() == oper[j])
						{
							if (oper[j] == ')')
							{
								st.pop();
								while (st.top() != '(')
								{
									cout << st.top();
									st.pop();
								}
								st.pop();
								st.push(equ[offset]);
							}
							else
							{
								if (weight[j] < weight[i] || equ[offset] =='(')
								{
									st.push(equ[offset]);
								}
								else
								{
									cout << st.top();
									st.pop();
									st.push(equ[offset]);
								}
							}
							break;
						}
					}

				}
				operflag = true;
				break;
			}
		}

		if (operflag == false)
			cout << equ[offset];
		offset++;
	}

	while (!st.empty())
	{
		if (st.top() != '(' && st.top() != ')')
			cout << st.top();
		st.pop();
	}

	return 0;
}