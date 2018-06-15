#include <iostream>

#include <queue>
#include <conio.h>
#include <stack>
#include "maze.h"

using namespace std;

class Position
{
public:
	int x;
	int y;
};

#define MAX MAZESIZE
#define PATH 0
#define WALL 1
#define VISITED 2
#define BACKTRACE 3


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

	if (maze[Next.y][Next.x] == PATH)
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
				cout << "P    ";
			}
			else
			{
				if (maze[y][x] >= 100)
				{
					cout << maze[y][x] << "\t";
				}
				else
				{
					cout << maze[y][x] << "\t";
				}
			}
		}
		cout << endl;
	}

	cout << "(" << Current.y << "," << Current.x << ")" << endl;
}

void PrintResultMaze()
{
	for (int y = 0; y < MAX; y++)
	{
		for (int x = 0; x < MAX; x++)
		{
			if (maze[y][x] == 0)
			{
				cout << 0 << " ";
			}
			else
			{
				cout << 1 << " ";
			}
		}
		cout << endl;
	}
}


bool CanTrace(const Position& Current, Position& NewPosition, int dir)
{
	Position Next;

	Next.x = Current.x + Dir[dir].x;
	Next.y = Current.y + Dir[dir].y;


	if (Next.x < 0 || Next.y < 0 || Next.x > MAX - 1 || Next.y > MAX - 1)
	{
		return false;
	}

	if (maze[Next.y][Next.x] == maze[Current.y][Current.x]-1)
	{
		NewPosition = Next;
		return true;
	}


	return false;
}


int main()
{
	queue<Position> Q;
	InitDir();
	Position Current;
	Current.x = 0;
	Current.y = 0;

	Q.push(Current);
	maze[0][0] = 100;
	bool found = false;

	while (!Q.empty())
	{
		Current = Q.front();
		Q.pop();
		int PrevLevel = maze[Current.y][Current.x];
		for (int i = 0; i < 4; ++i) //Up, Right, Down, Left
		{
			Position NewPosition;
			if (CanMove(Current, NewPosition, i))
			{
				maze[NewPosition.y][NewPosition.x] = maze[Current.y][Current.x] + 1;
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

	PrintMaze(Current);

	getch();

	stack<Position> TraceStack;

	int CurrentValue = maze[MAX - 1][MAX - 1];
	Position CurrentPos;
	CurrentPos.x = MAX - 1;
	CurrentPos.y = MAX - 1;

	TraceStack.push(CurrentPos);
	found = false;
	while(CurrentValue != 100)
	{
		for (int i = 0; i < 4; ++i) // ¹æÇâ
		{
			Position NewPosition;
			if (CanTrace(CurrentPos, NewPosition, i))
			{
				TraceStack.push(NewPosition);
				CurrentPos = NewPosition;
				found = true;
				break;
			}
		}
		if(!found)
		{
			maze[CurrentPos.y][CurrentPos.x] = WALL;
			CurrentPos = TraceStack.top();
			TraceStack.pop();
		}

		found = false;

		if (CurrentPos.x == 0 && CurrentPos.y == 0)
		{
			//cout << "¿Ï·á" << endl;
			break;
		}
	}

	while (!TraceStack.empty())
	{
		Position Way = TraceStack.top();
		TraceStack.pop();
		maze[Way.y][Way.x] = 0;
	}

	PrintResultMaze();

	return 0;
}