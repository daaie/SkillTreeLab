#include "maze.h"

void initDir()
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

void print(Position Current)
{
	for (int i = 0; i < MAXSIZE; i++)
	{
		for (int j = 0; j < MAXSIZE; j++)
		{
			if (Current.x == j && Current.y == i)
			{
				printf("P");
				continue;
			}
			else
			{
				if (maze[i][j] == WALL)
					printf("#");
				else if (maze[i][j] == BACKTRACE)
					printf("x");
				else
					printf("0");
			}

		}
		printf("\n");
	}

}
Position *move(Position current, int i)
{
	Position *next = new Position();
	next->x = current.x + Dir[i].x;
	next->y = current.y + Dir[i].y;

	if (next->x >= MAXSIZE || next->y >= MAXSIZE || next->x < 0 || next->y < 0)
	{
		return NULL;
	}

	if (maze[next->y][next->x] == PATH)
	{
		return next;
	}

	return NULL;
}

void printWay()
{
	Position next;
	Position current;

	stack<Position> cord;

	current.x = MAXSIZE - 1;
	current.y = MAXSIZE - 1;
	cord.push(current);

	while (maze[current.y][current.x] != INIT)
	{
		for (int i = 0; i < 4; i++)
		{
			next.x = current.x + Dir[i].x;
			next.y = current.y + Dir[i].y;

			if (next.x < MAXSIZE && next.y < MAXSIZE && next.x >= 0 && next.y >= 0)
				if (maze[next.y][next.x] == maze[current.y][current.x] - 1)
				{
					cord.push(next);
					current = next;
					break;
				}
		}
	}

	while (!cord.empty())
	{
		maze[cord.top().y][cord.top().x] = BACKTRACE;
		cord.pop();
	}

	print(current);
}

int main()
{
	stack<Position> cord;
	initDir();

	Position current;
	current.x = 0;
	current.y = 0;

	cord.push(current);
	maze[0][0] = INIT;

	while (!cord.empty())
	{
		current=cord.top();
		cord.pop();

		if (current.x == MAXSIZE - 1 && current.y == MAXSIZE - 1)
		{
			print(current);
			printf("≈ª√‚!!\n");
			printWay();
			return 0;
		}

		for (int i = 0; i < 4; i++)
		{
			if (Position *next = move(current, i))
			{
				maze[next->y][next->x] = maze[current.y][current.x] + 1;
				cord.push(*next);
			}
		}

		print(current);
		Sleep(50);
		system("cls");
	}

	printf("≈ª√‚Ω«∆–!!");
	return 0;
}