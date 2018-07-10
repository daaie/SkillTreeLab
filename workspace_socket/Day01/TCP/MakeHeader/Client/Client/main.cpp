#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib")

int main()
{
	WSADATA wsaData;
	SOCKET hSocket;
	SOCKADDR_IN servAddr;

	//socket make
	//connect
	//send , recv
	//rendering, processing

	char message[1024];
	char buffer[1024];

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		printf("winsocket error");
	}

	hSocket = socket(PF_INET, SOCK_STREAM, 0);
	if (hSocket == INVALID_SOCKET)
	{
		printf("socket error.");
	}

	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = inet_addr("127.0.0.1"); //192.168.0.200
	servAddr.sin_port = htons(3000); // 1 - 1024, http 80, ftp 21, ssh 22, https 433
									 // 1024 ~ 65535 
	
	if (connect(hSocket, (SOCKADDR*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)
	{
		printf("connect() error!");
	}
	else
	{
		printf("Connected...........");
	}

	while (1)
	{
		fputs("Input message(Q to quit): ", stdout);
		fgets(message, 1024, stdin);

		if (!strcmp(message, "q\n") || !strcmp(message, "Q\n"))
			break;
		int sendLength = strlen(message);
		int sendlen = htonl(strlen(message)); //문자열 길이 구함, 네트워크 바이트 오버
		//memcpy(buffer, &sendlen, 4); //문자열 길이 기록
		//memcpy(buffer+4, message, strlen(message)); //문자열 복사

		sendlen = send(hSocket, buffer, strlen(message)+4, 0);

		int recvlen = 0; //받은 전체 문자열 길이
		int recvcnt = 0; //받을때마 받은 문자열 길이
		while (recvlen < sendLength) // 보낸 문자열 길이 만큼 되야지 다 받은거
		{
			recvcnt = recv(hSocket, &message[recvcnt], strlen(message)- recvlen, 0);
			if (recvcnt == -1)
			{
				printf("recv error");
				break;
			}
			recvlen += recvcnt;
		}


		message[recvlen] = 0;
		printf("Message from server: %s", message);
	}

	closesocket(hSocket);
	WSACleanup();

	return 0;
}
