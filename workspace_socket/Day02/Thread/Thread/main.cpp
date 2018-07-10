#include <WinSock2.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#pragma comment(lib, "ws2_32.lib")

DWORD WINAPI inputThread(LPVOID arg);

SOCKET ServerSocket;
SOCKADDR_IN ServerAddr;
WSAData wsaData;

bool bIsRunning = true;

int main()
{
	HANDLE handle;
	DWORD threadID;
	int ArgTest = 0;

	if (WSAStartup(MAKEWORD(2, 2), &wsaData)!= 0 )
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

	handle = CreateThread(NULL,
		0,
		inputThread,
		(LPVOID)ArgTest,
		0,
		&threadID
		);

	char Buffer[1028];
	while (bIsRunning)
	{
		memset(Buffer, 0x00, sizeof(Buffer));
		int size = recv(ServerSocket, Buffer, sizeof(Buffer) - 1, 0);
		if(size == INVALID_SOCKET)
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
			printf("%s\n", Buffer);
		}
	}

	closesocket(ServerSocket);
	WSACleanup();
	printf("hello world");

	return 0;
}

// Input , send 해야함
// ServerSocket 필요함
DWORD WINAPI inputThread(LPVOID arg)
{
	/*for (int i = 0; i < 10; i++)
	{
		printf("%d\n", arg);
	}*/

	char InputBuffer[1024] = { 0 };
	while (true)
	{
		if (fgets(InputBuffer, 1024, stdin)) 
		{
			if (send(ServerSocket, InputBuffer, strlen(InputBuffer), 0) < 0)
			{
				printf("send error");

			}
			else
			{
				//클아이언트 종ㄹ료처리
				if (!strcmp(InputBuffer, "q=n") || !strcmp(InputBuffer, "Q\n"))
				{
					bIsRunning = false;
					break;
				}
			}
		}
	}

	//while (1)
	//{

	//}

	return 0;
}