#include <stdio.h>
#include <stdlib.h>
#include <WinSock2.h>

#pragma comment(lib, "ws2_32.lib")

#define BUFSIZE 1024
#define MAX_CLIENT 10

COORD Clients[MAX_CLIENT] = { 0,0 };

void ParseClientPos(char *Buffer, int index)
{
	char	*ptr;
	COORD Client;

	ptr = strtok(Buffer, ",");
	Client.X = atoi(ptr);
	ptr = strtok(NULL, ",");
	Client.Y = atoi(ptr);

	Clients[index] = Client;
}

void MakeBuffer(char* sendBuffer,char *Buffer, int i, int Length)
{
	char tempBuffer[1028] = {};
	char sendSize[4+1] = {};

	memset(sendBuffer, 0x00, BUFSIZE);
	sprintf(sendBuffer, "%d:", i);
	strncat(sendBuffer, Buffer, Length);

	sprintf(sendSize, "%4d", strlen(sendBuffer));

	memset(tempBuffer, 0x00, sizeof(tempBuffer));
	memcpy(tempBuffer, sendSize, 4);
	memcpy(tempBuffer+4, sendBuffer, strlen(sendBuffer));
	memcpy(sendBuffer, tempBuffer, strlen(tempBuffer));
}

int main()
{
	WSAData wsaData;
	SOCKET ClientSocket;
	SOCKET ServerSocket;
	SOCKADDR_IN ClientAddr;
	SOCKADDR_IN ServerAddr;

	TIMEVAL TimeOut;

	fd_set Reads;
	fd_set copyReads;

	char Buffer[BUFSIZE] = {};
	char sendBuffer[BUFSIZE] = {};

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		printf("WSAStartup error\n");
		exit(-1);
	}

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
	int fd_max = ServerSocket;
	int fdNum = 0;

	while (true)
	{
		copyReads = Reads;
		TimeOut.tv_sec = 5;
		TimeOut.tv_usec = 5000;

		if ((fdNum = select(0, &copyReads, 0, 0, &TimeOut)) == SOCKET_ERROR)
		{
			printf("select error\n");
		}

		if (fdNum == 0)
		{
			continue;
		}

		int AddressSize;
		for (int i = 0; i < Reads.fd_count; i++)
		{
			SOCKET CurrentSocket = Reads.fd_array[i];
			if (FD_ISSET(CurrentSocket, &copyReads))
			{
				if (CurrentSocket == ServerSocket)
				{
					AddressSize = sizeof(ClientAddr);
					ClientSocket = accept(ServerSocket, (SOCKADDR*)&ClientAddr, &AddressSize);
					FD_SET(ClientSocket, &Reads);
					printf("client connect %d \n", ClientSocket);

					for(int j=1;j<Reads.fd_count;j++)
					{
						memset(Buffer, 0x00, sizeof(Buffer));
						sprintf(Buffer, "%d,%d", Clients[j].X, Clients[j].Y);
						MakeBuffer(sendBuffer, Buffer, j, strlen(Buffer));
						if (Reads.fd_array[j]!= ClientSocket)
						{
							send(ClientSocket, sendBuffer, strlen(sendBuffer), 0);
							printf("Client %d send %s \n", ClientSocket, sendBuffer);
						}
					}
				}
				else
				{
					memset(Buffer, 0, BUFSIZE);
					int Length = recv(Reads.fd_array[i], Buffer, BUFSIZE - 1, 0);
					if (Length <= 0)
					{
						FD_CLR(Reads.fd_array[i], &Reads);
						closesocket(copyReads.fd_array[i]);
						printf("Client close %d \n", copyReads.fd_array[i]);

						MakeBuffer(sendBuffer, "-1,-1", i, 5);
						Clients[i] = { 0,0 };
						for (int j = 0; j < Reads.fd_count; j++)
						{
							if(j!=i)
							send(Reads.fd_array[j], sendBuffer, strlen(sendBuffer), 0);
						}
					}
					else
					{
						Buffer[Length] = '\0';
						MakeBuffer(sendBuffer, Buffer, i, Length);
						ParseClientPos(Buffer, i);
						for (int j = 0; j < Reads.fd_count; j++)
						{
							if(j!=i)
							send(Reads.fd_array[j], sendBuffer, strlen(sendBuffer) , 0);
						}
						printf("Client %d send %s \n", Reads.fd_array[i], sendBuffer);
					}
				}
			}
		}
	}
	closesocket(ServerSocket);
	WSACleanup();

	return 0;
}