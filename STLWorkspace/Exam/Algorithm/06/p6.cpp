#include "DLList.h"
#include "conio.h"

int main()
{
	DLList<char *> dList;
	
	FILE *fp = NULL;
	fp = fopen("Problem06.txt", "r");
	if (fp == NULL)
	{
		cout<<"파일없음"<<endl;
		return -1;
	}
	else
	{
		char *buffer;

		while (!feof(fp))
		{
			buffer = NULL;
			buffer = (char *)malloc(1024);
			fgets(buffer, 1024, fp);

			dList.AddNode(buffer);
		}

		int startLine = 1;
		int maxLine = dList.Length();
		char key=' ';

		while(key!='q')
		{
			for(int i=startLine;i<=maxLine;i++)
			{
				cout << dList.IndexOf(i)->Value;
				if (i-startLine > 5)
					break;
			}

			key = getch();
			switch (key)
			{
				case 'w' :
					if(startLine > 1)
					startLine--;
					break;
				case 's' :
					if (maxLine - startLine > 5)
					startLine++;
					break;
			}

			system("cls");
		}
	}
	return 0;
}
