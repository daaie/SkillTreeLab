#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <winsock2.h>

#include <windows.h>

#include <vector>
#include <list>

#include "player.h"
#include "utility.h"
#include "msgtype.h"
#include "sendata.h"

using namespace std;

#pragma comment(lib, "ws2_32.lib")

DWORD WINAPI RecvThread(LPVOID Arg);

//�÷��̾� ����Ʈ
vector<Player> PlayerList;

//�ڱ� �÷��̾�
Player LocalPlayer;

//���� ����
SOCKET ServerSocket;

//ũ��Ƽ�� ���� ����
CRITICAL_SECTION cs;


//���� ���� ������
bool isRunning = true;

//������ �÷��̾� �׸���
void DrawPlayer()
{
	for (int i = 0; i < (int)PlayerList.size(); ++i)
	{
		Utility::gotoxy(PlayerList[i].x, PlayerList[i].y);
		printf("%d", PlayerList[i].UserID);
	}
}



int main()
{
	WSADATA wsaData;
	SOCKADDR_IN ServerAddr;

	//ũ��Ƽ�� ���� �ʱ�ȭ
	InitializeCriticalSection(&cs);

	//���� �ʱ�ȭ
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		printf("WSAStartup() error!");
		exit(-1);
	}

	//���� ���� �ʱ�ȭ, �ּ� �ʱ�ȭ
	ServerSocket = socket(PF_INET, SOCK_STREAM, 0);
	memset(&ServerAddr, 0, sizeof(ServerAddr));
	ServerAddr.sin_family = PF_INET;
	ServerAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	ServerAddr.sin_port = htons(3000);

	//������ ����
	if (connect(ServerSocket, (SOCKADDR*)&ServerAddr, sizeof(ServerAddr)) == SOCKET_ERROR)
	{
		printf("Connect Error.\n");
		exit(-1);
	}
	
	

	//��Ʈ��ũ �ޱ� ������ ����
	HANDLE hThread;
	DWORD ThreadID;
	hThread = CreateThread(NULL,
		0,
		RecvThread,
		0,
		0,
		&ThreadID
	);

	//���� �÷��̾� �����
	PlayerList.push_back(LocalPlayer);

	while (isRunning)
	{
		//�Է� ó��
		int KeyCode = Utility::Input();
		//����
		if (KeyCode == 'q' || KeyCode == 'Q')
		{
			break;
		}
		
		//������ ó��
		PlayerList[0].Move(KeyCode);

		//������ ����
		SendData::Move(ServerSocket, PlayerList[0].x, PlayerList[0].y, PlayerList[0].UserID);
		
		//ȭ�� ����
		Utility::ClearScreen();
		//�÷��̾� �׸���
		DrawPlayer();
	}

	DeleteCriticalSection(&cs);

	return 0;
}

//��Ʈ��ũ ������
//�ٸ� Ŭ���̾�Ʈ �̵� ó��
DWORD WINAPI RecvThread(LPVOID Arg)
{
	//���
	char Header[2]; 
	//��Ŷ ����
	char Buffer[1024];
	while (true)
	{
		//header ����
		int RecvLen = recv(ServerSocket, Header, 2, 0);
		if (RecvLen == SOCKET_ERROR)
		{
			closesocket(ServerSocket);
			break;
		}
		else if (RecvLen == 0)
		{
			closesocket(ServerSocket);
			break;
		}
		else //��Ŷ �Ľ�
		{
			//��Ŷ ����
			char Method = Header[0];
			//��Ŷ ����
			char Length = Header[1];

			//��Ŷ �ٵ� �о� ����
			int BodyRecvLen = 0;
			while (BodyRecvLen < Length)
			{
				RecvLen = recv(ServerSocket, &Buffer[BodyRecvLen], Length, 0);
				if (RecvLen == 0 || RecvLen == SOCKET_ERROR)
				{
					closesocket(ServerSocket);
					isRunning = false;
					return 0;
				}
				else
				{
					BodyRecvLen += RecvLen;
				}
			}

			int x;
			int y;
			int id;
			bool insert = true;

			switch ((MSGType)Method)
			{
				//���� ���� �� ���� ó��
				case MSGType::LoginAck:
					memcpy(&id, &Buffer[0], 4);
					PlayerList[0].UserID = id; //�ڱ� ���̵� �Է�
					break;
				//�ٸ� ���� ����
				case MSGType::MakeUser:
					memcpy(&x, &Buffer[0], 4);
					memcpy(&y, &Buffer[4], 4);
					memcpy(&id, &Buffer[8], 4);

					//��� ���� ����Ʈ�� ���� ������
					//�̹� ������ ������ �������� ����
					insert = true;
					for (auto it : PlayerList)
					{
						if (it.UserID == id)
						{
							insert = false;
							break;
						}
					}
					if (insert)
					{
						Player NewPlayer;
						NewPlayer.x = x;
						NewPlayer.y = y;
						NewPlayer.UserID = id;

						//����ȭ ����
						EnterCriticalSection(&cs);
						PlayerList.push_back(NewPlayer);
						//����ȭ ��
						LeaveCriticalSection(&cs);
		
						Utility::Log(string("�ٸ� Ŭ���̾�Ʈ �ű� ���� �߰�"));
					}
					break;
				case MSGType::DeleteUser:
				{
					Utility::Log(string("�ٸ� ���� ����"));

					memcpy(&id, &Buffer[0], 4);
					auto it = PlayerList.begin();
					while (it != PlayerList.end())
					{
						if (it->UserID == id)
						{
							//����ȭ ����
							EnterCriticalSection(&cs);
							it = PlayerList.erase(it);
							//����ȭ ��
							LeaveCriticalSection(&cs);
						}
						else
						{
							++it;
						}
					}

				}
				break;

				case MSGType::Move:
				{
					memcpy(&x, &Buffer[0], 4);
					memcpy(&y, &Buffer[4], 4);
					memcpy(&id, &Buffer[8], 4);
					for (auto it = PlayerList.begin(); it != PlayerList.end(); ++it)
					{
						if (it->UserID == id)
						{
							//����ȭ ����
							EnterCriticalSection(&cs);
							it->x = x;
							it->y = y;
							//����ȭ ��
							LeaveCriticalSection(&cs);
						}
					}
				}
				break;
			}

			Utility::ClearScreen();
			DrawPlayer();
		}
	}

	isRunning = false;

	return 0;
}
