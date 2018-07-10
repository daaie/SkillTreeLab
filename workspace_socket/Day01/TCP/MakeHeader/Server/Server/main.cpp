#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib")

int main()
{
	//socket make
	//bind
	//listen
	//accept
	//send, recv

	WSADATA wsaData;
	SOCKET hSocket;
	SOCKADDR_IN servAddr;

	char message[1024];
	char buffer[1024];

	//윈도우 하는 윈속 초기화
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
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servAddr.sin_port = htons(3000); // 1 - 1024, http 80, ftp 21, ssh 22, https 433
	// 1024 ~ 65535

	if (bind(hSocket, (SOCKADDR*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)
	{
		printf("bind Error");
	}

	if (listen(hSocket, 5) == SOCKET_ERROR)
	{
		printf("listen error");
	}

	SOCKET hClientSocket;
	SOCKADDR_IN ClientAddr;
	int strLen = 1;
	int clntAdrSize = sizeof(ClientAddr);
	for (int i = 0; i<5; i++)
	{
		hClientSocket = accept(hSocket, (SOCKADDR*)&ClientAddr, &clntAdrSize);
		if (hClientSocket == -1)
		{
			printf("accept() error");
		}
		else
		{
			printf("Connected client %d \n", i + 1);
		}

		while (strLen != 0)
		{
			int stringLength;
			char header[4];
			strLen = recv(hClientSocket, header, 4, 0);
			memcpy(&stringLength, header, 4);
			stringLength = ntohl(stringLength); // 패킷 길이를 받아서 호스트 바이트 오더로

			int recvlen = 0; //받은 전체 문자열 길이, 헤더 길이는 기본 추가
			int recvcnt = 0; //받을때마 받은 문자열 길이
			while (recvlen < stringLength) // 보낸 문자열 길이 만큼 되야지 다 받은거
			{
				recvcnt = recv(hClientSocket, &message[recvcnt], stringLength - recvcnt, 0);
				if (recvcnt == -1)
				{
					printf("recv error");
					break;
				}
				recvlen += recvcnt;
			}

			send(hClientSocket, message, stringLength, 0);
		}
	}

	closesocket(hClientSocket);
	closesocket(hSocket);

	WSACleanup();

	return 0;
}
