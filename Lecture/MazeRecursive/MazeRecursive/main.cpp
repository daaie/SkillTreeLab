#include <stdio.h>
#include <stdlib.h>
#include <iostream>

using namespace std;

#define MAX 10

#define WAY 0
#define WALL 1
#define BLOCK 2
#define PATH 3



int Maze[MAX][MAX] =
{
	{ 0, 0, 0, 0, 1, 0, 0, 0, 0, 0 },
	{ 1, 1, 1, 0, 1, 0, 1, 1, 1, 0 },
	{ 1, 1, 1, 0, 1, 0, 1, 1, 1, 0 },
	{ 1, 1, 1, 0, 1, 0, 0, 0, 0, 0 },
	{ 1, 1, 1, 0, 1, 0, 1, 1, 0, 1 },
	{ 1, 0, 0, 0, 1, 0, 1, 1, 0, 0 },
	{ 1, 0, 1, 0, 1, 0, 0, 0, 1, 0 },
	{ 1, 0, 1, 0, 1, 0, 1, 0, 1, 0 },
	{ 0, 0, 1, 1, 1, 1, 1, 0, 1, 0 },
	{ 1, 0, 0, 0, 0, 0, 0, 0, 1, 0 }
};


bool FindMaze(int x, int y)
{
	if (x < 0 || y < 0 || x >= MAX || y >= MAX)
	{
		return false;
	}
	else if (Maze[y][x] != WAY)
	{
		return false;
	}
	else if (y == (MAX - 1) && x == (MAX - 1))
	{
		Maze[y][x] = PATH;
		return true;
	}
	else 
	{
		Maze[y][x] = PATH;
		if (FindMaze(x, y - 1) ||
			FindMaze(x + 1, y) ||
			FindMaze(x, y + 1) ||
			FindMaze(x - 1, y))
		{
			return true;
		}
		Maze[y][x] = BLOCK;
		return false;
	}
}

void PrintMaze()
{
	for (int y = 0; y < MAX; y++)
	{
		for (int x = 0; x < MAX; x++)
		{
			cout << Maze[y][x] << " ";
		}
		cout << endl;
	}
}


int main()
{
	PrintMaze();
	FindMaze(0, 0);
	cout << endl;
	PrintMaze();
}