#include "stdio.h"
#include "stdlib.h"
#include "LStack.h"
#include "AStack.h"

/*
int main()
{
	LStack
	LStack<int> a;

	a.Push(1);
	a.Push(2);
	a.Push(3);
	a.Push(4);
	a.Push(5);
	a.Push(6);
	a.Push(7);
	a.Push(8);

	int b = a.Pop();
	printf("pop : %d\n", b);
	b=a.Pop();
	printf("pop : %d\n", b);

	while (a.Length() > 0)
	{
		printf("%d\n", a.Pop());
	}

	LStack<char> c;
	for (int i = 'Z'; i >= 'A'; --i)
	{
		c.Push(i);
	}

	printf("peek : %c , lenght : %d \n", c.Peek(), c.Length());

	while (c.Length() > 0)
	{
		printf("%c\n", c.Pop());
	}
	

	//AStack<int> a;
	//a.Init();

	//for (int i = 10; i >= 1; --i)
	//{
	//	a.Push(i);
	//}

	//int c;

	//do
	//{
	//	c = a.Pop();
	//	printf("%d\n", c);

	//} while (a.Length() != 0);
	
	return 0;
}
*/

// -----------------중위 표기법, 후위 표기법 
// 괄호없고
// 띄어쓰기로 문자 숫자 구분

char Operators[]="+-/*()";
int OperatorPriority[] = { 1,1,2,2,0,0 };
AStack<char*> PostFixStack;

int flag = 0;

int IsOperator(char ch)
{
	for(int i = 0;  i < strlen(Operators); i++)
	{
		if (ch == Operators[i])
			return i;
	}
	return -1;
}

int GetPriority(char Operator)
{
	return OperatorPriority[IsOperator(Operator)];
}

char* OperatorProcess(char *token, char * Cursor)
{
	if (token[0] == '(')
	{
		flag = 1;
	}

	if (token[0] == ')')
		flag = 0;

	if (PostFixStack.Length() == 0 || flag == 1)
	{
		if(token[0] != '(')
		PostFixStack.Push(token);
	}
	else
	{
		char *Top = PostFixStack.Peek();
		while (!(PostFixStack.Length() == 0) &&	GetPriority(Top[0]) >= GetPriority(token[0]))
		{
			PostFixStack.Pop();
			sprintf(Cursor, "%s ", Top);
			Cursor += (strlen(Top) + 1);

			if (!PostFixStack.Length() == 0)
			{
				Top = PostFixStack.Peek();
			}
		}
		if(token[0] != ')')
		PostFixStack.Push(token);
	}
	return Cursor;
}

char *ConvertPostFix(char * Infix)
{
	PostFixStack.Init();
	char *PostFixString = new char[strlen(Infix) + 1];
	char *Cursor = PostFixString;

	char* token = strtok(Infix, " ");
	while (token != NULL)
	{
		if (token[0] >= '0' && token[0] <= '9')
		{
			sprintf(Cursor, "%s ", token);
			Cursor += strlen(token) + 1;
		}
		else if (IsOperator(token[0]) != -1)
		{
			Cursor = OperatorProcess(token, Cursor);
		}
		else
		{
			printf("error\n");
			return NULL;
		}

		token = strtok(NULL, " ");
	}

	while (!(PostFixStack.Length() == 0))
	{
		char *Pop = PostFixStack.Pop();
		sprintf(Cursor, "%s ", Pop);
		Cursor += (strlen(Pop) + 1);
	}

	sprintf(Cursor, "\0");

	return PostFixString;
}
double PostFixCalculate(char *PostFixString)
{
	AStack<char *> PostFixCal;
	PostFixCal.Init();

	char* token = strtok(PostFixString, " ");
	while (token != NULL)
	{
		if (token[0] >= '0' && token[0] <= '9')
		{
			PostFixCal.Push(token);
		}
		else if (IsOperator(token[0]) != -1)
		{
			char *first = PostFixCal.Pop();

			char *second = PostFixCal.Pop();
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
			PostFixCal.Push(temp);
		}
		else
		{
			printf("error\n");
			return 0;
		}

		token = strtok(NULL, " ");
	}

	char *final = PostFixCal.Pop();
	
	return atof(final);
}

int main()
{
	char Infix[] = "( 150 + 60 / 2 ) * 2 + ( 78 - 20 + 60 ) + 1";
	char *PostFixString = ConvertPostFix(Infix);

	printf("%s\n", PostFixString);

	double result = PostFixCalculate(PostFixString);
	
	printf("result : %f\n", result);

	return 0;
}
