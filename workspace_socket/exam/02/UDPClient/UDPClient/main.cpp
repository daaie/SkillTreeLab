#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#define BUF_SIZE 30

#pragma comment(lib, "ws2_32.lib")

int main(int argc, char *argv[])
{
	WSADATA wsaData;
	SOCKET servSock;

	char message[BUF_SIZE];
	int strLen;
	int clntAdrSz;

	SOCKADDR_IN servAdr;

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		printf("startup error");

	}
	servSock = socket(PF_INET, SOCK_DGRAM, 0);
	if (servSock == INVALID_SOCKET)
		printf("servSOck error");

	memset(&servAdr, 0, sizeof(servAdr));
	servAdr.sin_family = AF_INET;
	servAdr.sin_addr.s_addr = inet_addr("127.0.0.1");
	servAdr.sin_port = htons(3000);
	connect(servSock, (SOCKADDR*)&servAdr, sizeof(servAdr));

	while (1)
	{
		fputs("Insert Text: ", stdout);
		fgets(message, sizeof(message), stdin);
		if (!strcmp(message, "q\n"))
			break;

		send(servSock, message, strlen(message), 0);
		strLen = recv(servSock, message, sizeof(message) - 1, 0);
		message[strLen] = 0;
		printf("message : %s\n", message);
	}
	closesocket(servSock);
	WSACleanup();
	return 0;
}