#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>


//int sum_execute(int* array, int* outSum, int* outAvg);
//
//int main()
//{
//	int sum = 0;
//	int i = 0;
//	int avg = 0;
//
//	int num[10];
//	for (int i = 0; i < 3; ++i)
//	{
//		scanf("%d", &num[i]);
//	}
//
//	if (sum_execute(num, &sum, &avg) == 0)
//	{
//		printf("%d\n", sum);
//		printf("%d\n", avg);
//	}
//	else
//	{
//		printf("error\n");
//	}
//
//
//	return 0;
//}
//
//int sum_execute(int* array, int* outSum, int* outAvg)
//{
//	*outSum = 0;
//	for (int i = 0; i < 3; ++i)
//	{
//		*outSum += array[i];
//	}
//
//	*outAvg = (*outSum) / 3;
//
//	return 0;
//}

//int main()
//{
//	char* p;
//	p = (char*)malloc(3*sizeof(int)); 
//
//	if (p)
//	{
//		p[0] = 1;
//		p[1] = 2;
//		*(p + 2) = 3;
//
//		p[3] = 4;
//
//		printf("%d", p[3]);
//	}
//	else
//	{
//		//error
//	}
//
//
//
//	//if (p) free(p);
//}

//����ȭ���� �Ǿ����ϴ�.
//int main()
//{
//	char c[] = "Hello World";
//	char* c2 = "Hello World";
//
//	c[0] = 'K';
//	c2[0] = 'K';
//
//	printf("%s\n", c); 
//	printf("%s\n", c2);
//
//
//	return 0;
//}

//int main()
//{
//	char* words[10];
//	char buffer[1024] = { 0 };
//
//	for (int i = 0; i < 3; ++i)
//	{
//		scanf("%s", buffer);
//		int l = strlen(buffer);
//		words[i] = (char*)malloc(l * sizeof(char));
//		strcpy(words[i], buffer);
//	}
//
//	for (int i = 0; i < 3; ++i)
//	{
//		printf("%s\n", words[i]);
//	}
//}

int main()
{
	char* words[100];		//�ڷ� ���� �迭
	char buffer[1024];		//�Է� �ӽ� �����
	int MaxCount = 100;		//�ִ� ����
	int Count = 0;			//�Էµ� ����

	FILE* fp = fopen("c:\\data.txt", "r");
	FILE* fout = fopen("output.txt", "w");

	do
	{
		buffer[0] = 0;
		//gets_s(buffer, 1024);
		//fgets(buffer, 1024, stdin); //Ű����(�⺻�Է�) �Է�
		fgets(buffer, 1024, fp); //���� �Է�

		int WordsCount = strlen(buffer); //���ڿ� ����
		if (WordsCount > 0) //�Է��� �Ѱ��� ������
		{
			buffer[WordsCount - 1] = 0; // �ٹٲ� ���� ����
			printf("%s:%d\n", buffer, WordsCount); //���
			words[Count] = strdup(buffer); // ���� ������ �޸� ���� ����
			Count++;	//���� ����
		}

	} while (strlen(buffer) != 0 && Count < MaxCount); //�ִ� ������ ���� �ʰ� �Է��� ������ ����

	for (int i = 0; i < Count; ++i) //���
	{
		int WordsCount = strlen(words[i]);
		//printf("%s:%d\n", words[i], WordsCount);
		fprintf(fout, "%s:%d\n", words[i], WordsCount);
	}

	fclose(fp);
	fclose(fout);
}