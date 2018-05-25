#include "DLList.h"
#include <list>
#include <conio.h>
using namespace PdaDLList;

class Line
{
public :
	Line() {

	}
	char data[81];
};

int main()
{
	//DLList<int> DL;
	//DL.Push_back(1);
	//DL.Push_back(2);
	//DL.Push_back(3);
	//DL.Push_back(4);
	//DL.Push_back(5);
	//DL.Push_back(6);

	//DL.Print();

	//DLLNode<int>* Cursor = DL.IndexOf(6);
	//printf("%d\n", Cursor->Value);


	//----------------------------------------------------------------------------------
	//복사 생성자 공부 - 클래스를 template로 받기
	//Line l1;
	//l1.data[0] ='a';
	//Line l2;
	//l2.data[0] = 'b';

	//DLList<Line> T;

	//T.Push_back(l1);
	//T.Push_back(l2);


	//printf("%c\n", T.IndexOf(2)->Value.data[0]);
	//---------------------------------------------------------------------------------------
	//파일 읽기
	
	DLList<Line> T;
	FILE* fp = fopen("data.txt", "r");
	char buffer[1024] = { 0 };

	if (fp == nullptr)
	{
		printf("파일이없어...\n");
	}
	else
	{
		Line Temp;
		//while(!feof(fp))
		while (fgets(buffer, sizeof(buffer), fp) != NULL)
		{
			memset(Temp.data, 0x00, sizeof(Temp.data));
			memcpy(Temp.data, buffer, sizeof(Temp.data)-1);
			T.Push_back(Temp);
		}

		//for(DLLNode<Line> *Current = T.Indexof(1); Current! = T.End(); Current = Current->Next)
		//for (int i = 1; i <= T.count; i++)
		//{
		//	if (i > 5) break;
		//	printf("%s", T.IndexOf(i)->Value.data);
		//}

		char InputKey;
		int StartLine = 1;
		int MaxLine = T.Length();
		int PageSize = 10;
		do
		{
			for (int i = StartLine; i <= T.Length(); i++)
			{
				if (i - StartLine > PageSize)
				{
					break;
				}
				printf("%s", T.IndexOf(i)->Value.data);
			}

			InputKey = getch(); //->conio.h
			switch (InputKey)
			{ 
				case 'w' :
					StartLine--;
					if (StartLine < 1)
						StartLine = 1;
					break;
				case 's' :
					StartLine++; 
					if (StartLine > MaxLine - PageSize)
						StartLine = MaxLine - PageSize;
					break;
			}
			system("cls");

		} while (InputKey != 'q');

		fclose(fp);
	}

	return 0;
}