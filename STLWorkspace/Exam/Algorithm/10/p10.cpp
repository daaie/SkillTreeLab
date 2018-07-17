#include <iostream>
#include <stack>

using namespace std;

int main()
{
	char *equ = (char *)malloc(1024);
	cout<<"식 입력 (공백으로 구분): ";
	scanf("%[^\n]",equ);
	
	stack<char *> cal;
	
	char* token = strtok(equ, " ");
	while (token != NULL)
	{
		if (token[0] >= '0' && token[0] <= '9')
		{
			cal.push(token);
		}
		else if (token[0] == '+' ||
				 token[0] == '-' ||
				 token[0] == '*' ||
				 token[0] == '/' )
		{
			char *first = cal.top();
			cal.pop();

			char *second = cal.top();
			cal.pop();

			double result = 0;
			
			switch (token[0])
			{
				case '+':
					result = atof(second) + atof(first);
					break;
				case '-':
					result = atof(second) - atof(first);
					break;
				case '*':
					result = atof(second) * atof(first);
					break;
				case '/':
					result = atof(second) / atof(first);
					break;
			}
			char *temp = new char[1024];
			sprintf(temp, "%f", result);
			cal.push(temp);
		}
		else
		{
			printf("error\n");
			return 0;
		}

		token = strtok(NULL, " ");
	}

	cout << "result : "<< cal.top() <<endl;
	

	return 0;
}