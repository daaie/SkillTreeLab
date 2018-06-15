#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "AStack.h"


char Infix[] = "2 - 10 / 5.1 * ( 6 + 4 )";
char Operators[] = "+-/*()";
int OperatorPriority[] = { 1, 1, 2, 2, -1, -1 };

AStack<char*> PostFixStack;

//������ ���� �˾ƺ��� �Լ�
//return : �����ڸ� �迭 ��ġ
//�����ڰ� �ƴϸ� -1
int IsOperator(char ch)
{
	for (int i = 0; i < (int)strlen(Operators); ++i)
	{
		if (ch == Operators[i])
		{
			return i; // ������ �迭 ��ġ
		}
	}

	return -1; //�����ڰ� �ƴϸ� -1
}

//�迭 ��ġ�� ���� ������ �켱���� ��������
int GetPriority(char* Operator)
{
	return OperatorPriority[IsOperator(Operator[0])];
}


///
//1. ����ǥ����� ó�� �� ������ �б�
//2 - 1 ����(�ǿ�����) ���
//2 - 2 ������(+-*/ ) ���� Push
//2 - 2 - 1 ���� �������� �켱������ ����� �������� �켱�������� ���ų� ������ �� ������(POP) ���
//2 - 2 - 2 ���� ��ȣ��('(') ������ Push
//2 - 2 - 3 �ݴ� ��ȣ��(')') ������ ���� ��ȣ�� ���ö� ���� POP �ϰ� �ݴ� ��ȣ, ���� ��ȣ�� ����
//3 ����ǥ��� ���� �� �о����� ���ÿ� �ִ°� �� ������ ���
//4. ���ÿ� �� ������ ��� ���
char* OperatorProcess(char* Operator, char* Cursor)
{
	if ((PostFixStack.Length() == 0) || Operator[0] == '(') //ù �����ڸ� ������ Ǫ��
	{
		PostFixStack.Push(Operator);
	}
	else //ù �����ڰ� �ƴϸ� 
	{
		char* Top = PostFixStack.Peek(); //�� �� ������ Ȯ��
		//���� ó�� �ϴ� �������� �켱�������� ���ų� ������ ���
		while (!(PostFixStack.Length() == 0) && GetPriority(Top) >= GetPriority(Operator))
		{
			PostFixStack.Pop(); // �׳� ������, �̹� �� ���� �����ڴ� �˰� ����.
			if (Top[0] == '(')
			{
				break;
			}
			//printf("%s ", Top);
			sprintf(Cursor, "%s ", Top);
			Cursor += (strlen(Top) + 1);

			if (!(PostFixStack.Length() == 0))
			{
				Top = PostFixStack.Peek(); //������ �� ������ ã�ƿ���
			}
		}
		//���� ó�� �ϴ� ������ Ǫ��
		if (Operator[0] != ')')
		{
			PostFixStack.Push(Operator);
		}
	}

	return Cursor;
}

char* ConvertPostFix(char* InfixNotation)
{
	PostFixStack.Init();

	//���� ����� ��������� ���� �迭
	char* PostFixString = new char[strlen(InfixNotation) + 1];
	//���� ��� ���� ��ġ Ŀ��
	char* Cursor = PostFixString;

	//���� ǥ��� �߶� �о� ����(�Ľ�)
	char* token = strtok(InfixNotation, " ");
	while (token != NULL)
	{
		if (token[0] >= '0' && token[0] <= '9') // ���� �϶� ó��, ���
		{
			//printf("%s ", token);
			sprintf(Cursor, "%s ", token);
			Cursor += (strlen(token) + 1);
		}
		else if (IsOperator(token[0]) != -1) //������ ó��, ���ÿ� Ǫ��(����)
		{
			Cursor = OperatorProcess(token, Cursor);
		}
		else // ���� ó��
		{
			printf("error");
			return NULL;
		}

		token = strtok(NULL, " ");
	}

	//���ÿ� ���� �ִ� ������ ��� ���
	while (!(PostFixStack.Length() == 0))
	{
		char* Pop = PostFixStack.Pop();
		//printf("%s ", Pop);
		sprintf(Cursor, "%s ", Pop);
		Cursor += (strlen(Pop) + 1);
	}

	//���ڿ� �� �ڴ� '\0'
	sprintf(Cursor, "\0");

	return PostFixString;
}

double PostFixCalculate(char* PostFixString)
{
	AStack<char*> PostFixCal;
	PostFixCal.Init();

	//����ǥ��� �Ľ�
	char* token = strtok(PostFixString, " ");
	while (token != NULL)
	{
		if (token[0] >= '0' && token[0] <= '9') // ���� �϶� ó��
		{
			PostFixCal.Push(token); //���ڴ� ������ ���ÿ� Ǫ��
		}
		else if (IsOperator(token[0]) != -1) //������ ó��
		{
			char* second = PostFixCal.Pop(); //���� �ι�° ������
			char* first = PostFixCal.Pop(); //���� ù��° ������

			//���� ó��
			double result = 0;
			switch (token[0])
			{
			case '+':
				result = atof(first) + atof(second);
				break;
			case '-':
				result = atof(first) - atof(second);
				break;
			case '/':
				result = atof(first) / atof(second);
				break;
			case '*':
				result = atof(first) * atof(second);
				break;
			}
			//���� ��� ���� Ǫ��
			char* temp = new char[10];
			sprintf(temp, "%f", result);
			PostFixCal.Push(temp);
		}
		else // ���� ó��
		{
			printf("error");
			return NULL;
		}

		token = strtok(NULL, " ");
	}

	//�������� ����� ����� ���
	char* Final = PostFixCal.Pop();
	//printf("%f\n", atof(Final));

	return atof(Final);
}


int main()
{
	//printf("%c\n", ('0'-1));
	//printf("%c\n", '0');
	//printf("%c\n", '1');
	//printf("%c\n", '2');
	//printf("%c\n", '3');
	//printf("%c\n", '4');
	//printf("%c\n", '5');
	//printf("%c\n", '6');
	//printf("%c\n", '7');
	//printf("%c\n", '8');
	//printf("%c\n", '9');
	//printf("%c\n", ('9'+1));

	char* PostFixString = ConvertPostFix(Infix);
	printf("%s\n", PostFixString);
	printf("%f\n", PostFixCalculate(PostFixString));

	return 0;
}