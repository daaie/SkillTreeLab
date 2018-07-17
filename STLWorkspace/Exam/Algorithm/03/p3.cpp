#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
	char *str = (char *)malloc(sizeof(char) * 100);

	printf("문자입력 : ");
	scanf("%s", str);
	
	printf("문자열의 길이 : %d\n", strlen(str));

	return 0;
}
