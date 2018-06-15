#include <iostream>
//#include "AQueue.h"
#include <queue>
#include <conio.h>

using namespace std;

class Position
{
public:
	int x;
	int y;
};

#define MAX 10
#define PATH 0
#define WALL 1
#define VISITED 2
#define BACKTRACE 3

int Maze[MAX][MAX] =
{
	{ 0, 0, 0, 0, 1, 0, 0, 0, 0, 0 },
	{ 0, 1, 1, 0, 1, 0, 1, 1, 1, 0 },
	{ 1, 1, 1, 0, 1, 0, 1, 1, 1, 0 },
	{ 1, 1, 1, 0, 1, 0, 1, 1, 0, 0 },
	{ 1, 1, 1, 0, 1, 0, 1, 1, 0, 1 },
	{ 1, 0, 0, 0, 1, 0, 1, 1, 0, 0 },
	{ 1, 0, 1, 0, 1, 0, 0, 0, 1, 0 },
	{ 1, 0, 1, 0, 1, 0, 1, 0, 1, 0 },
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
};

Position Dir[4];

void InitDir()
{
	//Up
	Dir[0].x = 0;
	Dir[0].y = -1;

	//Right
	Dir[1].x = 1;
	Dir[1].y = 0;

	//Down
	Dir[2].x = 0;
	Dir[2].y = 1;

	//Left
	Dir[3].x = -1;
	Dir[3].y = 0;
}


bool CanMove(const Position& Current, Position& NewPosition, int dir)
{
	Position Next;

	Next.x = Current.x + Dir[dir].x;
	Next.y = Current.y + Dir[dir].y;

	
	if (Next.x < 0 || Next.y < 0 || Next.x > MAX-1 || Next.y > MAX-1)
	{
		return false;
	}

	if (Maze[Next.y][Next.x] == PATH)
	{
		NewPosition = Next;
		return true;
	}


	return false;
}

void PrintMaze(const Position& Current)
{
	for (int y = 0; y < MAX; y++)
	{
		for (int x = 0; x < MAX; x++)
		{
			if (Current.x == x && Current.y == y)
			{
				cout << "P" << "\t";
			}
			else
			{
				cout << Maze[y][x] << "\t";
			}
		}
		cout << endl;
	}

	cout << "(" << Current.y << "," << Current.x << ")" << endl;
}


int main()
{
	queue<Position> Q;
	InitDir();
	Position Current;
	Current.x = 0;
	Current.y = 0;

	Q.push(Current);
	Maze[0][0] = 100;
	bool found = false;

	while (!Q.empty())
	{
		Current = Q.front();
		Q.pop();
		int PrevLevel = Maze[Current.y][Current.x];
		for (int i = 0; i < 4; ++i) //Up, Right, Down, Left
		{
			Position NewPosition;
			if (CanMove(Current, NewPosition, i))
			{
				Maze[NewPosition.y][NewPosition.x] = Maze[Current.y][Current.x] + 1;
				if (NewPosition.x == MAX - 1 && NewPosition.y == MAX - 1)
				{
					cout << "Å»Ãâ" << endl;
					found = true;
					break;
				}

				Q.push(NewPosition);
			}
		}

		system("cls");
		PrintMaze(Current);
		getch();

		if (found)
		{
			break;
		}
	}

	if (!found)
	{
		cout << "Å»Ãâ ºÒ°¡" << endl;
	}





	return 0;
}