#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
	char *str = (char *)malloc(sizeof(char) * 100);

	printf("�����Է� : ");
	scanf("%s", str);
	
	printf("���ڿ��� ���� : %d\n", strlen(str));

	return 0;
}
