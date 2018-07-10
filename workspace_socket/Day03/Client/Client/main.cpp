#include <WinSock2.h>
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>
#include <Windows.h>

#pragma comment(lib, "ws2_32.lib")

#define BUFSIZE 1024
#define STAND 0
#define LEFT 75
#define RIGHT 77
#define DOWN 80
#define UP 72
#define MAX_CLIENT 10

typedef struct Client {
	COORD Pos;
	bool isSet;
};

bool bIsRunning = true;
COORD g_Current = {0,0};
Client g_Pos[MAX_CLIENT] = {0,false};
int g_Index = 0;

void print()
{
	system("cls");

	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), g_Current);
	printf("☆");

	for (int i = 0; i<MAX_CLIENT; i++)
	{
		if (g_Pos[i].isSet == true)
		{ 
			SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), g_Pos[i].Pos);
			printf("@");
		}
	}
}

bool Move()
{
	int Input;

	Input = getch();
	if (Input == 224)
	{
		Input = getch();
		switch (Input)
		{
		case LEFT:
			if (g_Current.X > 0)
				g_Current.X -= 1;
			break;
		case RIGHT:
			g_Current.X += 1;
			break;
		case UP:
			if (g_Current.Y-1 >= 0)
				g_Current.Y -= 1;
			break;
		case DOWN:
			g_Current.Y += 1;
			break;
		default:
			break;
		}
		print();
		return true;
	}
	return false;
}

DWORD WINAPI recvThread(LPVOID arg)
{
	char recvBuffer[BUFSIZE] = {};
	SOCKET ServerSocket = (SOCKET)arg;

	int BufferSize = 0;
	char sizeBuffer[4] = {};

	while (bIsRunning)
	{
		memset(recvBuffer, 0x00, BUFSIZE);

		if (recv(ServerSocket, sizeBuffer, 4, 0) == 4)
		{
			BufferSize = atoi(sizeBuffer);

			int size = recv(ServerSocket, recvBuffer, BufferSize, 0);
			if (size == INVALID_SOCKET)
			{
				printf("socket error");
				exit(-1);
			}
			else if (size == 0)
			{
				printf("connect close");
				closesocket(ServerSocket);
				exit(-1);
			}
			else
			{
				char	*ptr;
				COORD Client;

				ptr = strtok(recvBuffer, ":");
				g_Index = atoi(ptr);
				ptr = strtok(NULL, ",");
				if (atoi(ptr) == -1)
				{
					g_Pos[g_Index].Pos = { 0,0 };
					g_Pos[g_Index].isSet = false;
					print();
					continue;
				}
				else
				{
					Client.X = atoi(ptr);
				}

				ptr = strtok(NULL, ",");
				Client.Y = atoi(ptr);

				g_Pos[g_Index].Pos = Client;
				g_Pos[g_Index].isSet = true;

				print();
			}
		}
	}

	return 0;
}

int main()
{
	// 소켓변수
	SOCKADDR_IN ServerAddr;
	WSAData wsaData;
	SOCKET ServerSocket;
	// 스레드변수
	HANDLE handle;
	DWORD threadID[2];

	char sendBuffer[BUFSIZE] = {};

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		printf("WSAStartup error");
		exit(-1);
	}

	memset(&ServerAddr, 0x00, sizeof(ServerAddr));
	ServerAddr.sin_family = PF_INET;
	ServerAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	ServerAddr.sin_port = htons(3000);

	ServerSocket = socket(PF_INET, SOCK_STREAM, 0);

	if (connect(ServerSocket, (SOCKADDR*)&ServerAddr, sizeof(ServerAddr)) == SOCKET_ERROR)
	{
		printf("connect error");
		exit(-1);
	}

	print();

	handle = CreateThread(NULL,
		0,
		recvThread,
		(LPVOID)ServerSocket,
		0,
		&threadID[0]
	);

	while (bIsRunning)
	{
		if (Move())
		{
			memset(sendBuffer, 0x00, BUFSIZE);
			sprintf(sendBuffer, "%d,%d", g_Current.X, g_Current.Y);
			if (send(ServerSocket, sendBuffer, strlen(sendBuffer), 0) < 0)
			{
				printf("send error");

			}
		}
	}

	closesocket(ServerSocket);
	WSACleanup();

	return 0;
}
