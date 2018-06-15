#include <stack>
#include <iostream>
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

Position Dir[4];


bool CanMove(Position& Current)
{
	//if (Current.y - 1 >= 0 && Maze[Current.y - 1][Current.x] == 0) // Up
	//{
	//	Current.y--;
	//	return true;
	//}
	//else if (Current.x + 1 < MAX && Maze[Current.y][Current.x + 1] == 0) // Right
	//{
	//	Current.x++;
	//	return true;
	//}
	//else if (Current.y + 1 < MAX &&  Maze[Current.y + 1][Current.x] == 0) // Down
	//{
	//	Current.y++;
	//	return true;
	//}
	//else if (Current.x - 1 >= 0 && Maze[Current.y][Current.x - 1] == 0) // Left
	//{
	//	Current.x--;
	//	return true;
	//}

	Position Next;

	for (int i = 0; i < 4; ++i)
	{
		Next.x = Current.x + Dir[i].x;
		Next.y = Current.y + Dir[i].y;

		if (Next.x < 0 || Next.y < 0 || Next.x > MAX || Next.y > MAX)
		{
			continue;
		}

		if (Maze[Next.y][Next.x] == PATH)
		{
			Current = Next;
			return true;
		}
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
				cout << "P" << " ";
			}
			else
			{
				cout << Maze[y][x] << " ";
			}
		}
		cout << endl;
	}

	cout << "(" << Current.y << "," << Current.x << ")" << endl;
}


int main()
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


	stack<Position> Map;
	Position Current;
	Current.x = 0;
	Current.y = 0;

	while (true)
	{
		Maze[Current.y][Current.x] = VISITED;
		if (!CanMove(Current))
		{
			if (!Map.empty())
			{
				Position Temp = Map.top();
				Map.pop();
				Maze[Current.y][Current.x] = BACKTRACE;
				Current = Temp; // 되돌아 가기
			}
			else
			{
				cout << "탈출불가" << endl;
				break;
			}
		}
		else
		{
			Map.push(Current);
		}


		PrintMaze(Current);
		getch();
		system("cls");

		if (Current.x == (MAX - 1) && Current.y == MAX - 1)
		{
			cout << "탈출" << endl;
			break;
		}
	}


	return 0;
}