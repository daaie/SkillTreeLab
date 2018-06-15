#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "AStack.h"


char Infix[] = "2 - 10 / 5.1 * ( 6 + 4 )";
char Operators[] = "+-/*()";
int OperatorPriority[] = { 1, 1, 2, 2, -1, -1 };

AStack<char*> PostFixStack;

//연산자 인지 알아보는 함수
//return : 연산자면 배열 위치
//연산자가 아니면 -1
int IsOperator(char ch)
{
	for (int i = 0; i < (int)strlen(Operators); ++i)
	{
		if (ch == Operators[i])
		{
			return i; // 연산자 배열 위치
		}
	}

	return -1; //연산자가 아니면 -1
}

//배열 위치에 따라서 연산자 우선순위 가져오기
int GetPriority(char* Operator)
{
	return OperatorPriority[IsOperator(Operator[0])];
}


///
//1. 중위표기법을 처음 한 단위씩 읽기
//2 - 1 숫자(피연산자) 출력
//2 - 2 연산자(+-*/ ) 스택 Push
//2 - 2 - 1 현재 연산자의 우선순위가 저장된 연산자의 우선순위보다 높거나 같으면 다 꺼내서(POP) 출력
//2 - 2 - 2 여는 괄호면('(') 무조건 Push
//2 - 2 - 3 닫는 괄호면(')') 무조건 여는 괄호가 나올때 까지 POP 하고 닫는 괄호, 여는 괄호는 버림
//3 중위표기법 수식 다 읽었으면 스택에 있는거 다 꺼내서 출력
//4. 스택에 들어간 연산자 모두 출력
char* OperatorProcess(char* Operator, char* Cursor)
{
	if ((PostFixStack.Length() == 0) || Operator[0] == '(') //첫 연산자면 무조건 푸시
	{
		PostFixStack.Push(Operator);
	}
	else //첫 연산자가 아니면 
	{
		char* Top = PostFixStack.Peek(); //젤 위 연산자 확인
		//지금 처리 하는 연산자의 우선순위보다 높거나 같으면 출력
		while (!(PostFixStack.Length() == 0) && GetPriority(Top) >= GetPriority(Operator))
		{
			PostFixStack.Pop(); // 그냥 버리기, 이미 젤 위에 연산자는 알고 있음.
			if (Top[0] == '(')
			{
				break;
			}
			//printf("%s ", Top);
			sprintf(Cursor, "%s ", Top);
			Cursor += (strlen(Top) + 1);

			if (!(PostFixStack.Length() == 0))
			{
				Top = PostFixStack.Peek(); //다음에 볼 연산자 찾아오기
			}
		}
		//지금 처리 하는 연산자 푸시
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

	//최종 결과로 후위연산식 저장 배열
	char* PostFixString = new char[strlen(InfixNotation) + 1];
	//최종 결과 저장 위치 커서
	char* Cursor = PostFixString;

	//중위 표기법 잘라서 읽어 오기(파싱)
	char* token = strtok(InfixNotation, " ");
	while (token != NULL)
	{
		if (token[0] >= '0' && token[0] <= '9') // 숫자 일때 처리, 출력
		{
			//printf("%s ", token);
			sprintf(Cursor, "%s ", token);
			Cursor += (strlen(token) + 1);
		}
		else if (IsOperator(token[0]) != -1) //연산자 처리, 스택에 푸시(조건)
		{
			Cursor = OperatorProcess(token, Cursor);
		}
		else // 예외 처리
		{
			printf("error");
			return NULL;
		}

		token = strtok(NULL, " ");
	}

	//스택에 남아 있는 연산자 모두 출력
	while (!(PostFixStack.Length() == 0))
	{
		char* Pop = PostFixStack.Pop();
		//printf("%s ", Pop);
		sprintf(Cursor, "%s ", Pop);
		Cursor += (strlen(Pop) + 1);
	}

	//문자열 젤 뒤는 '\0'
	sprintf(Cursor, "\0");

	return PostFixString;
}

double PostFixCalculate(char* PostFixString)
{
	AStack<char*> PostFixCal;
	PostFixCal.Init();

	//후위표기법 파싱
	char* token = strtok(PostFixString, " ");
	while (token != NULL)
	{
		if (token[0] >= '0' && token[0] <= '9') // 숫자 일때 처리
		{
			PostFixCal.Push(token); //숫자는 무조건 스택에 푸시
		}
		else if (IsOperator(token[0]) != -1) //연산자 처리
		{
			char* second = PostFixCal.Pop(); //숫자 두번째 꺼내기
			char* first = PostFixCal.Pop(); //숫자 첫번째 꺼내기

			//연산 처리
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
			//연산 결과 스택 푸시
			char* temp = new char[10];
			sprintf(temp, "%f", result);
			PostFixCal.Push(temp);
		}
		else // 예외 처리
		{
			printf("error");
			return NULL;
		}

		token = strtok(NULL, " ");
	}

	//마지막에 저장된 결과를 출력
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