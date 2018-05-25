#include <stack>
#include <iostream>
#include <conio.h>

using namespace std;

#define MAX 10
#define PATH 0
#define WALL 1
#define VISITED 2
#define BACKTRACE 3

class Position
{
public:
	int x;
	int y;
};

int MAZE[MAX][MAX] =
{
	{ 0,1,1,0,1,1,0,0,0,0 },
	{ 0,1,1,0,1,1,1,1,1,0 },
	{ 0,1,0,0,0,0,0,1,1,0 },
	{ 1,1,1,1,1,1,0,1,1,0 },
	{ 1,1,1,0,0,0,0,1,1,0 },
	{ 1,1,1,1,1,1,0,1,1,0 },
	{ 1,1,1,1,1,1,0,1,1,0 },
	{ 1,1,1,1,1,1,0,0,0,0 },
	{ 1,1,1,1,1,1,1,1,1,0 },
	{ 1,1,1,1,1,1,1,1,1,0 }
};

Position Dir[4] = {0};

void Print(Position Current)
{
	for (int i = 0; i < MAX; i++)
	{
		for (int j = 0; j < MAX; j++)
		{
			if (Current.x == j && Current.y == i)
			{
				printf("P");
				continue;
			}
			else
			{
				if (MAZE[i][j] == 1)
					printf("#");
				else if (MAZE[i][j] == 0)
					printf("0");
				else if (MAZE[i][j] == 3)
					printf("@");
				else
					printf("9");
			}

		}
		printf("\n");
	}

}
/*
bool Move(Position& Current) //C++�� �����͸����̰ž�^__________^
{
	if (Current.y -1 >= 0 && MAZE[Current.y - 1][Current.x] == 0) // &&������ �տ��� Ʋ���� �ڿ��� ����������..������
	{
		Current.y--;
		return true;
	}else if (Current.x+ 1 < MAX && MAZE[Current.y][Current.x+1] == 0)
	{
		Current.x++;
		return true;
	}else if (Current.y + 1 < MAX && MAZE[Current.y +1][Current.x] == 0)
	{
		Current.y++;
		return true;
	}
	else if (Current.x - 1 >= 0 && MAZE[Current.y][Current.x-1] == 0)
	{
		Current.x--;
		return true;
	}
	return false;
}

int main()
{
	stack<Position> st;

	Position Current;
	Current.x = 0;
	Current.y = 0;

	while (1)
	{
		MAZE[Current.y][Current.x] = VISITED;

		if (!Move(Current))
		{
			Position Temp = st.top();
			st.pop();
			MAZE[Current.y][Current.x] = BACKTRACE;
			Current = Temp;
		}
		else
		{
			st.push(Current);
		}


		Print(Current);
		getch();
		system("cls");

		if (Current.x == MAX - 1 && Current.y == MAX - 1)
		{
			cout << "Ż��" << endl;
			break;
		}
	}

	return 0;
}
*/

bool Move(Position& Current)
{
	Dir[0].x = 0;
	Dir[0].y = -1;

	Dir[1].x = 1;
	Dir[1].y = 0;

	Dir[2].x = 0;
	Dir[2].y = 1;

	Dir[3].x = -1;
	Dir[3].y = 0;

	Position Next;
	for (int i = 0; i < 4; i++)
	{
		Next.x = Current.x + Dir[i].x;
		Next.y = Current.y + Dir[i].y;

		if (Next.x < 0 || Next.y < 0 || Next.x >= MAX || Next.y >= MAX)
		{
			continue;
		}

		if (MAZE[Next.y][Next.x] == PATH)
		{
			Current=Next;
			return true;
		}
	}

	return false;
}
int main()
{
	stack<Position> st;

	Position Current;
	Current.x = 0;
	Current.y = 0;

	while (1)
	{
		MAZE[Current.y][Current.x] = VISITED;

		if (!Move(Current))
		{
			if (st.empty())
			{
				cout << "Ż��Ұ�" << endl;
				break;
			}

			Position Temp = st.top();
			st.pop();
			MAZE[Current.y][Current.x] = BACKTRACE;
			Current = Temp;

		}
		else
		{
			st.push(Current);
		}

		Print(Current);
		getch();

		if (Current.x == MAX - 1 && Current.y == MAX - 1)
		{
			cout << "Ż��" << endl;
			break;
		}

		system("cls");
	}

	return 0;
}