#include <iostream>
#include <conio.h>
#include "AQueue.h"
#include <Windows.h>
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
	{ 0,0,0,0,0,0,0,1,1,0 },
	{ 1,1,1,1,1,1,0,1,1,0 },
	{ 1,1,1,0,0,0,0,1,1,0 },
	{ 1,1,1,1,1,1,0,1,1,0 },
	{ 1,1,1,1,1,1,0,1,1,0 },
	{ 1,1,1,1,1,1,0,0,0,0 },
	{ 1,1,1,1,1,1,1,1,1,0 },
	{ 1,1,1,1,1,1,1,1,1,0 }
};

Position Dir[4] = { 0 };

void InitDir()
{
	Dir[0].x = 0;
	Dir[0].y = -1;

	Dir[1].x = 1;
	Dir[1].y = 0;

	Dir[2].x = 0;
	Dir[2].y = 1;

	Dir[3].x = -1;
	Dir[3].y = 0;
}

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
				//if (MAZE[i][j] == 1)
				//	printf("#");
				//else //if (MAZE[i][j] == 0)
				//	printf("0");
				//else if (MAZE[i][j] == 3)
				//	printf("@");
				//else
				//	printf("9");

				printf("%d", MAZE[i][j]);
				printf("\t");
			}

		}
		printf("\n");
	}

}

bool Move(const Position current, Position &newPosition, int dir)
{
	Position Next;

	Next.x = current.x + Dir[dir].x;
	Next.y = current.y + Dir[dir].y;

	if (Next.x < 0 || Next.y < 0 || Next.x >= MAX || Next.y >= MAX)
	{
		return false;
	}

	if (MAZE[Next.y][Next.x] == PATH)
	{
		newPosition = Next;
		return true;
	}

	return false;
}

int main()
{
	Aqueue<Position> q;
	q.init(1000);
	InitDir();
	
	Position current;
	current.x = 0;
	current.y = 0;

	q.enqueue(current);
	MAZE[0][0] = 100;
	bool found = false;

	while (!q.isEmpty())
	{
		q.dequeue(current);
		for (int i = 0; i < 4; i++)
		{
			Position newPosition;
			if (Move(current, newPosition, i))
			{
				MAZE[newPosition.y][newPosition.x] = MAZE[current.y][current.x] + 1;
				if (newPosition.x == MAX-1 && newPosition.y == MAX-1 )
				{
					Print(current);
					Sleep(1000);
					system("cls");
					Print(newPosition);
					cout << "Å»Ãâ" << endl;
					found = true;
					return 0;
				}
				q.enqueue(newPosition);
			}
		}
		Print(current);
		Sleep(1000);
		system("cls");
	}
	printf("Å»ÃâºÒ°¡!\¤Ì");

	return 0;
}