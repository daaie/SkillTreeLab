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

	SOCKADDR_IN servAdr, clntAdr;
	if (argc != 2) {
		printf("usage : %s port\n", argv[0]);
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
	servAdr.sin_addr.s_addr = htonl(INADDR_ANY);
	servAdr.sin_port = htons(atoi(argv[1]));

	if (bind(servSock, (SOCKADDR*)&servAdr, sizeof(servAdr)) == SOCKET_ERROR)
	{
		printf("bind error");
	}

	while (1)
	{
		clntAdrSz = sizeof(clntAdr);
		strLen = recvfrom(servSock, message, BUF_SIZE, 0,(SOCKADDR*)&clntAdr, &clntAdrSz);
		sendto(servSock, message, strLen, 0, (SOCKADDR*)&clntAdr, sizeof(clntAdr));
	}
	closesocket(servSock);
	WSACleanup();
	return 0;
}