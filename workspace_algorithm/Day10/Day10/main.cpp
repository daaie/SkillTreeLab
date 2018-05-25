#include <stdio.h>
#include <string.h>
#include "AStack.h"


using namespace pdaios;

int main()
{
	AStack<char> a;
	
	char str[100] = {0};
	//char *str=new char[100];
	printf("¹®ÀÚ¿­?\n");
	scanf("%s", str);
	
	a.Init(strlen(str));
	
	for (int i = 0; i < strlen(str); i++)
	{
		a.Push(str[i]);
	}
	
	for (int i=a.Length(); i>0; i--)
	{
		printf("%c",a.Pop());
	}

	printf("\n");

	return 0;
}