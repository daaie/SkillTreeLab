#include <iostream>
#include <Windows.h>

using namespace std;

#define MAX 10

#define IMAGE 1
#define BACKGROUND 0
#define COUNTIMAGE 2


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
			if (Maze[y][x] == BACKGROUND)
				cout << "■";
			else if (Maze[y][x] == COUNTIMAGE)
				cout << "□";
			else
				cout << "＠";
		}
		cout << endl;
	}
}

int CountImage(int x, int y)
{
	if (x < 0 || y < 0 || x >= MAX || y >= MAX)
	{
		return 0;
	}
	else if (Maze[y][x] != IMAGE)
	{
		return 0;
	}
	else
	{
		Maze[y][x] = COUNTIMAGE;
		return 1 +
			CountImage(x, y - 1) +
			CountImage(x + 1, y) +
			CountImage(x, y + 1) +
			CountImage(x - 1, y) +
			CountImage(x - 1, y - 1) +
			CountImage(x + 1, y + 1) +
			CountImage(x - 1, y + 1) +
			CountImage(x + 1, y - 1);
	}
}

int main()
{
	cout << "(0,1) 개수 : ";
	cout << CountImage(0, 1)<<endl;
	cout << "(4,0) 개수 : ";
	cout << CountImage(4, 0) << endl;
	cout << "(0,9) 개수 : ";
	cout << CountImage(0, 9) << endl;
	printMaze();
	return 0;
}