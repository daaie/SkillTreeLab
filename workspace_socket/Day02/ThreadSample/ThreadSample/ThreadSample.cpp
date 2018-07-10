#include <WinSock2.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#pragma comment(lib, "ws2_32.lib")

DWORD WINAPI InputThread(LPVOID Arg);

SOCKET ServerSocket;
SOCKADDR_IN ServerAddr;
WSADATA WsaData;

bool bIsRunning = true;

int main()
{
	HANDLE hThread;
	DWORD ThreadID;

	//윈속 초기화
	if (WSAStartup(MAKEWORD(2, 2), &WsaData) != 0)
	{
		exit(-1);
	}

	memset(&ServerAddr, 0, sizeof(ServerAddr));
	ServerAddr.sin_family = PF_INET;
	ServerAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	ServerAddr.sin_port = htons(3000);

	ServerSocket = socket(PF_INET, SOCK_STREAM, 0);

	if (connect(ServerSocket, (SOCKADDR*)&ServerAddr, sizeof(ServerAddr)) == SOCKET_ERROR)
	{
		printf("connect Error");
		exit(-1);
	}

	//입력 처리 쓰레드 시작
	hThread = CreateThread(NULL,
		0,
		InputThread,
		0,
		0,
		&ThreadID
	);

	char Buffer[1024];
	while (bIsRunning)
	{
		memset(Buffer, 0, 1024);
		int size = recv(ServerSocket, Buffer, 1024 - 1, 0);
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
			printf("Server to : %s\n", Buffer);
		}
	}

	closesocket(ServerSocket);
	WSACleanup();
	
	return 0;
}

//Input, Send
//ServerSocket
DWORD WINAPI InputThread(LPVOID Arg)
{
	char InputBuffer[1024];
	while (true)
	{
		if (fgets(InputBuffer, 1024, stdin))
		{
			if (send(ServerSocket, InputBuffer, strlen(InputBuffer), 0) < 0)
			{
				printf("send error");
				exit(-1);
			}
			else
			{
				if (!strcmp(InputBuffer, "q\n") || !strcmp(InputBuffer, "Q\n"))
				{
					bIsRunning = false;
					break;
				}
			}
		}
	}

	return 0;
}
