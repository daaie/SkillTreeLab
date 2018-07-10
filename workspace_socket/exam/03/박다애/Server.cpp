#include <stdio.h>
#include <stdlib.h>
#include <WinSock2.h>

#pragma comment(lib, "ws2_32.lib")

int main()
{
	WSAData wsaData;
	SOCKET ClientSocket;
	SOCKET ServerSocket;
	SOCKADDR_IN ClientAddr;
	SOCKADDR_IN ServerAddr;

	// select 함수 타임아웃 시간 구조체
	TIMEVAL TimeOut;

	// 멀티플렉싱을 위한 구조체
	fd_set Reads;
	fd_set copyReads;

	// 주소 구조체 길이용 변수
	int AddressSize;
	// 자료 임시 저장용 변수
	char Buffer[1024];


	// 윈속 초기화
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		printf("WSAStartup error\n");
		exit(-1);
	}

	// 서버 소켓 만들기
	ServerSocket = socket(PF_INET, SOCK_STREAM, 0);
	
	memset(&ServerAddr, 0, sizeof(ServerAddr));
	ServerAddr.sin_family = AF_INET;
	ServerAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	ServerAddr.sin_port = htons(3000);

	if (bind(ServerSocket, (SOCKADDR*)&ServerAddr, sizeof(ServerAddr)) == SOCKET_ERROR)
	{
		printf("Socket bind error \n");
		exit(-1);
	}

	if (listen(ServerSocket, 5) == SOCKET_ERROR)
	{
		printf("listen error \n");
		exit(-1);
	}

	FD_ZERO(&Reads);
	FD_SET(ServerSocket, &Reads);

	char sendBuffer[1028] = {};

	while (true)
	{
		//memcpy(&copyReads, &Reads, sizeof(Reads));
		copyReads = Reads;
		TimeOut.tv_sec = 5;
		TimeOut.tv_usec = 5000; //ms
		
		int fdNum = 0;
		if ((fdNum = select(0, &copyReads, 0, 0, &TimeOut)) == SOCKET_ERROR)
		{
			printf("select error\n");
			exit(-1);
		}

		if (fdNum == 0)
		{
			printf("select Timeout\n");
			continue;
		}

		for (int i = 0; i < Reads.fd_count; i++)
		{
			if (FD_ISSET(Reads.fd_array[i], &copyReads))
			{
				if (Reads.fd_array[i] == ServerSocket) // 클라이언트가 연결 요청을 함.
				{
					AddressSize = sizeof(ClientAddr);
					ClientSocket = accept(ServerSocket, (SOCKADDR*)&ClientAddr, &AddressSize);
					// 연결된 소켓을 입출력 감지에 등록.
					FD_SET(ClientSocket, &Reads);
					printf("client connect %d \n", i);
				}
				else // 서버가 정보를 받음, recv처리
				{
					memset(Buffer, 0, 1024);
					int Length = recv(Reads.fd_array[i], Buffer, 1024 - 1, 0);
					Buffer[Length] = '\0';
					if (Length == 0)// 연결종료
					{
						// 소켓 감지리스트에서 제거
						FD_CLR(Reads.fd_array[i], &Reads);
						closesocket(copyReads.fd_array[i]);
						printf("Clinet close %d \n", i);
					}
					else
					{
						printf("Clinet send %s \n", Buffer);
						memset(sendBuffer, 0x00, sizeof(sendBuffer));
						sprintf(sendBuffer, "%d:", i);
						strncat(sendBuffer, Buffer, Length);
						for (int j = 0; j < Reads.fd_count; j++)
							send(Reads.fd_array[j], sendBuffer, Length + 2, 0);
					}
				}
			}
		}
	}
	closesocket(ServerSocket);
	WSACleanup();

	return 0;
}