#include <stdio.h>
#include <stdlib.h>
#include <WinSock2.h>
#include <vector>
#include <string.h>
#include "msgtype.h"
#include "player.h"
#include "sendata.h"

#pragma comment(lib, "ws2_32.lib")


using namespace std;

//�÷��̾� ���� ����Ʈ
vector<Player> PlayerList;
//map<SOCKET, Player> PlayerListMap;

int main()
{
	//���� ��ü ������
	WSAData WsaData;
	//Ŭ���̾�Ʈ ���� ��ȣ
	SOCKET ClientSocket;
	//���� ���� ��ȣ
	SOCKET ServerSocket;
	//Ŭ���̾�Ʈ �ּ� ����
	SOCKADDR_IN ClientAddr;
	//���� �ּ� ����
	SOCKADDR_IN ServerAddr;

	//select �Լ� Ÿ�� �ƿ� �ð� ����ü
	TIMEVAL Timeout;

	//��Ƽ�÷����� ���� ����ü
	fd_set Reads;
	fd_set CopyReads;

	//�ּ� ����ü ���̿� ����
	int AddressSize;
	//�ڷ� �ӽ� �����
	char Buffer[1024];

	//��Ŷ �����
	char Header[2];


	//���� �ʱ�ȭ
	if (WSAStartup(MAKEWORD(2, 2), &WsaData) != 0)
	{
		printf("Winsock2 Error\n");
		exit(-1);
	}

	//�������� �����
	ServerSocket = socket(PF_INET, SOCK_STREAM, 0);
	//���� �ּ� ����ü ����
	memset(&ServerAddr, 0, sizeof(ServerAddr));
	ServerAddr.sin_family = AF_INET;
	ServerAddr.sin_addr.s_addr = htonl(INADDR_ANY); //���� �ƹ� �ּҳ� ��, ��ī�� �������� ��Ȯ�� �ؾߵ�
	ServerAddr.sin_port = htons(3000);

	//���� ���ε�
	if (bind(ServerSocket, (SOCKADDR*)&ServerAddr, sizeof(ServerAddr)) == SOCKET_ERROR)
	{
		printf("Socket bind error\n");
		exit(-1);
	}

	//���� ��Ʈ���� ��� ����
	if (listen(ServerSocket, 5) == SOCKET_ERROR)
	{
		printf("Socket listen error\n");
		exit(-1);
	}


	//����� �˸��� ���� ����ü �ʱ�ȭ
	FD_ZERO(&Reads);

	//����� �˸��� ���� ����ü�� OS ���, ���� ���Ͽ� ����
	FD_SET(ServerSocket, &Reads);

	int fdNum = 0;

	Utility::Log(string("���� ����"));

	while (true)
	{
		CopyReads = Reads;

		//���� Ÿ�� �ƿ� �ð�
		Timeout.tv_sec = 10;
		Timeout.tv_usec = 0;

		//Socket���� Read�� ����
		if ((fdNum = select(0, &CopyReads, 0, 0, &Timeout)) == SOCKET_ERROR)
		{
			printf("select error\n");
			exit(-1);
		}

		if (fdNum == 0) //Timeout
		{
			continue;
		}

		for (int i = 0; i < (int)Reads.fd_count; ++i)
		{
			//���� ��ȭ ����
			if (FD_ISSET(Reads.fd_array[i], &CopyReads))
			{
				if (Reads.fd_array[i] == ServerSocket) //Ŭ���̾�Ʈ�� ���� ��û�� ��
				{
					AddressSize = sizeof(ClientAddr);
					ClientSocket = accept(ServerSocket,
						(SOCKADDR*)&ClientAddr, &AddressSize);
					//����� Ŭ���̾�Ʈ ������ ����� ������ ���
					FD_SET(ClientSocket, &Reads);
					printf("Client connect %d\n", ClientSocket);


					Utility::Log(string("�ű� ���� ����"));

					//Send LoginAck
					//���� �������� ID �߱�
					SendData::LoginAck(ClientSocket, ClientSocket);
					
					//����Ʈ�� �ű� ���� �߰�
					Player NewPlayer;
					NewPlayer.x = 0;
					NewPlayer.y = 0;
					NewPlayer.UserID = ClientSocket;
					NewPlayer.ClientSocket = ClientSocket;
					PlayerList.push_back(NewPlayer);

					//������ �ִ� ���� �����ϱ�
					for (auto send : PlayerList)
					{
						for (auto it : PlayerList)
						{
							SendData::MakeUser(send.ClientSocket,
								it.x,
								it.y,
								it.UserID
							);
						}
					}
				}
				else //������ ������ ����, recvó��
				{
					//���� �ʱ�ȭ
					memset(Buffer, 0, 1024);

					//����� ����
					int RecvLen = recv(Reads.fd_array[i], Header, 2, 0);
					//int Length = recv(Reads.fd_array[i], Buffer, 1024 - 1, 0);
					if (RecvLen == 0 || RecvLen == SOCKET_ERROR) //���� ����
					{
						for (auto it = PlayerList.begin(); it != PlayerList.end();)
						{
							if (it->ClientSocket == Reads.fd_array[i])
							{
								it = PlayerList.erase(it);
							}
							else
							{
								++it;
							}
						}

						for (auto it = PlayerList.begin(); it != PlayerList.end(); ++it)
						{
							SendData::DeleteUser(it->ClientSocket, Reads.fd_array[i]);
						}

						//���� ���� ����Ʈ���� ����
						FD_CLR(Reads.fd_array[i]/*����� Ŭ���̾�Ʈ ����*/, &Reads);
						
						closesocket(CopyReads.fd_array[i]);
						printf("Client Close %d\n", CopyReads.fd_array[i]);
					}
					else //�޼��� ���°�
					{
						//��� ó��
						//int RecvLen = recv(Reads.fd_array[i], Header, 2, 0);
						//�޼��� ����
						char Method = Header[0];
						//�޼��� ����
						char Length = Header[1];

						//�޼��� �ٵ� �о� ����
						int BodyRecvLen = 0;
						RecvLen = 0;
						while (BodyRecvLen < Length) //TCP�� �ѹ��� �� �޾� �ü��� �־ ������ �о ��Ŷ�� ����.
						{
							RecvLen = recv(Reads.fd_array[i], &Buffer[BodyRecvLen], Length, 0);
							if (RecvLen == 0 || RecvLen == SOCKET_ERROR)
							{
								Utility::Log(string("���� ����"));

								closesocket(Reads.fd_array[i]);
								return 0;
							}
							else
							{
								BodyRecvLen += RecvLen;
							}
						}
						
						//�޼��� ó��
						switch ((MSGType)Method)
						{
							//�̵�
							case MSGType::Move:
							{
								Utility::Log(string("���� �̵�"));
								int x;
								int y;
								int id;

								//�̵� ��Ŷ �Ľ�
								memcpy(&x, &Buffer[0], 4);
								memcpy(&y, &Buffer[4], 4);
								memcpy(&id, &Buffer[8], 4);
								
								//���� ���� ������Ʈ
								for (auto it = PlayerList.begin(); it != PlayerList.end(); ++it)
								{
									if (it->UserID == id)
									{
										it->x = x;
										it->y = y;
									}
								}

								//������Ʈ �� ������ �ٸ� �������� ����
								for (auto it : PlayerList)
								{
									if (it.UserID != id) //�ڱ� �ڽ����״� ������ �ʴ´�.
									{
										SendData::Move(it.ClientSocket, x, y, id);
									}
								}
							}
						}
					}
				}
			}
		}
	}

	//���� ���� ����, ���� ���̺귯�� ����
	closesocket(ServerSocket);
	WSACleanup();

	return 0;
}