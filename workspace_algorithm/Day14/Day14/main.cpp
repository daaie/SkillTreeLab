#include <iostream>
#include <Windows.h>
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

void printMaze()
{
	for (int y = 0; y < MAX; y++)
	{
		for (int x = 0; x < MAX; x++)
		{
			if (Maze[y][x] == WALL)
				cout << "#";
			else if (Maze[y][x] == BLOCK)
				cout << "@";
			else if (Maze[y][x] == PATH)
				cout << "x";
			else
				cout << "0";
		}
		cout << endl;
	}
}

bool findMaze(int x, int y)
{
	if (x < 0 || y < 0 || x >= MAX || y >= MAX)
	{
		return false;
	}
	else if (Maze[y][x] != WAY)
	{
		return false;
	}
	else if (y == MAX - 1 && x == MAX - 1)
	{
		Maze[y][x] = PATH;
		return true;
	}
	else 
	{
		Maze[y][x] = PATH;
		if (findMaze(x, y - 1) ||
			findMaze(x + 1, y) ||
			findMaze(x, y + 1) ||
			findMaze(x - 1, y))
		{
			return true;
		}
		Maze[y][x] = BLOCK;
		return false;
	}
}

int main()
{
	printMaze();
	findMaze(0, 0);
	printMaze();
	return 0;
}