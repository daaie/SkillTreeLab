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
	if (argc != 3) {
		printf("usage : %s ip port\n", argv[0]);
		exit(1);
	}
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		printf("startup error");

	}
	servSock = socket(PF_INET, SOCK_DGRAM, 0); // udp 전송지향.. tcp연결지향`
	if (servSock == INVALID_SOCKET)
		printf("servSOck error");

	memset(&servAdr, 0, sizeof(servAdr));
	servAdr.sin_family = AF_INET;
	servAdr.sin_addr.s_addr = inet_addr(argv[1]);
	servAdr.sin_port = htons(atoi(argv[2]));
	connect(servSock, (SOCKADDR*)&servAdr, sizeof(servAdr));

	while (1)
	{
		fputs("insert : ", stdout);
		fgets(message, sizeof(message), stdin);
		if (!strcmp(message, "q\n"))
			break;

		send(servSock, message, strlen(message), 0);
		strLen = recv(servSock, message, sizeof(message)-1,0);
		message[strLen] = 0;
		printf("message : %s\n", message);
	}
	closesocket(servSock);
	WSACleanup();
	return 0;
}