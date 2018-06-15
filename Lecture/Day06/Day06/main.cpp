#include <stdio.h>
#include <stdlib.h>
#include "List.h"
#include <list>
#include <conio.h>

using namespace junios;

class Line
{
public:
	char data[81];
};


int main()
{
	List<Line> T;

	char buffer[81] = { 0 };

	FILE* fp = fopen("data.txt", "r");

	if (fp != nullptr)
	{
		while (!feof(fp))
		{
			memset(buffer, 0, 81);
			fgets(buffer, 80, fp);
			Line Temp;
			memcpy(Temp.data, buffer, 80);
			T.PushBack(Temp);
		}

		fclose(fp);
	}

	char InputKey;
	int StartLine = 1;
	int MaxLine = T.Length();
	int PageSize = 10;
	do
	{
		int i = 0;
		for (Node<Line>* Current = T.IndexOf(StartLine); Current != T.End(); Current = Current->Next)
		{	
			i++;
			printf("%s", Current->Value.data);
			if (i > PageSize)
			{
				break;
			}
		}
		InputKey = getch();
		if (InputKey == 'w')
		{
			StartLine--;

			if (StartLine < 1)
			{
				StartLine = 1;
			}
		}
		else if (InputKey == 's')
		{
			StartLine++;
			if (StartLine > MaxLine - PageSize)
			{
				StartLine = MaxLine - PageSize;
			}
		}
		system("cls");
	} while( InputKey != 'q');


	return 0;
}