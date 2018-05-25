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

//무궁화꽃이 피었습니다.
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
	char* words[100];		//자료 저장 배열
	char buffer[1024];		//입력 임시 저장소
	int MaxCount = 100;		//최대 갯수
	int Count = 0;			//입력된 갯수

	FILE* fp = fopen("c:\\data.txt", "r");
	FILE* fout = fopen("output.txt", "w");

	do
	{
		buffer[0] = 0;
		//gets_s(buffer, 1024);
		//fgets(buffer, 1024, stdin); //키보드(기본입력) 입력
		fgets(buffer, 1024, fp); //파일 입력

		int WordsCount = strlen(buffer); //문자열 세기
		if (WordsCount > 0) //입력이 한개라도 있으면
		{
			buffer[WordsCount - 1] = 0; // 줄바꿈 문자 제거
			printf("%s:%d\n", buffer, WordsCount); //출력
			words[Count] = strdup(buffer); // 버퍼 내용을 메모리 복사 생성
			Count++;	//갯수 증가
		}

	} while (strlen(buffer) != 0 && Count < MaxCount); //최대 갯수가 넘지 않고 입력이 없을때 까지

	for (int i = 0; i < Count; ++i) //출력
	{
		int WordsCount = strlen(words[i]);
		//printf("%s:%d\n", words[i], WordsCount);
		fprintf(fout, "%s:%d\n", words[i], WordsCount);
	}

	fclose(fp);
	fclose(fout);
}