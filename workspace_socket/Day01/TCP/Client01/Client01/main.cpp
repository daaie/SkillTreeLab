#include <stdio.h>
#include <stdlib.h>
#include <WinSock2.h>

#pragma comment(lib,"ws2_32.lib")
#define BUF_SIZE 1024
int main()
{
	//socket make
	//connect
	//send,recv

	WSADATA wsaData;
	SOCKET hSocket;
	SOCKADDR_IN servAddr;
	
	char message[1024] = {};
	char buffer[1024] = {};

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		printf("winsocket error");
	}

	hSocket = socket(PF_INET, SOCK_STREAM, 0);
	if (hSocket == INVALID_SOCKET)
	{
		printf("socket error.");
	}

	memset(&servAddr, 0x00, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	servAddr.sin_port = htons(3000);

	if (connect(hSocket, (SOCKADDR*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)
	{
		printf("connect error");
	}

	while(1)
	{
		fputs("input message ", stdout);
		fgets(message, BUF_SIZE, stdin);

		if (!strcmp(message, "q\n") || !strcmp(message, "Q\n"))
			break;

		int sendlength = strlen(message);
		//int sendlen = htonl(strlen(message));
		//memcpy(buffer, &sendlen, 4); //문자열길이기록
		//memcpy(buffer+4, message, sendlen); //문자열복사

		int sendLen = send(hSocket, buffer, strlen(message), 0);
		int recvLen = 0;
		int recvCnt = 0;
		while (recvLen < sendlength)
		{
			recvLen = recv(hSocket, &message[recvCnt], strlen(message) - recvLen, 0);
			if (recvCnt == -1)
			{
				printf("recv error");
			}
			recvLen += recvCnt;

		}
		message[recvLen] = 0x00;
		printf("message from server: ", message);
	}

	closesocket(hSocket);
	WSACleanup();

	return 0;
}