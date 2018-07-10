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

//플레이어 리스트
vector<Player> PlayerList;

//자기 플레이어
Player LocalPlayer;

//서버 소켓
SOCKET ServerSocket;

//크리티컬 섹션 변수
CRITICAL_SECTION cs;


//게임 루프 실행중
bool isRunning = true;

//접속한 플레이어 그리기
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

	//크리티컬 섹션 초기화
	InitializeCriticalSection(&cs);

	//윈속 초기화
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		printf("WSAStartup() error!");
		exit(-1);
	}

	//서버 소켓 초기화, 주소 초기화
	ServerSocket = socket(PF_INET, SOCK_STREAM, 0);
	memset(&ServerAddr, 0, sizeof(ServerAddr));
	ServerAddr.sin_family = PF_INET;
	ServerAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	ServerAddr.sin_port = htons(3000);

	//서버에 연결
	if (connect(ServerSocket, (SOCKADDR*)&ServerAddr, sizeof(ServerAddr)) == SOCKET_ERROR)
	{
		printf("Connect Error.\n");
		exit(-1);
	}
	
	

	//네트워크 받기 쓰레드 시작
	HANDLE hThread;
	DWORD ThreadID;
	hThread = CreateThread(NULL,
		0,
		RecvThread,
		0,
		0,
		&ThreadID
	);

	//로컬 플레이어 만들기
	PlayerList.push_back(LocalPlayer);

	while (isRunning)
	{
		//입력 처리
		int KeyCode = Utility::Input();
		//종료
		if (KeyCode == 'q' || KeyCode == 'Q')
		{
			break;
		}
		
		//움직임 처리
		PlayerList[0].Move(KeyCode);

		//움직임 전송
		SendData::Move(ServerSocket, PlayerList[0].x, PlayerList[0].y, PlayerList[0].UserID);
		
		//화면 삭제
		Utility::ClearScreen();
		//플레이어 그리기
		DrawPlayer();
	}

	DeleteCriticalSection(&cs);

	return 0;
}

//네트워크 쓰레드
//다른 클라이언트 이동 처리
DWORD WINAPI RecvThread(LPVOID Arg)
{
	//헤더
	char Header[2]; 
	//패킷 버퍼
	char Buffer[1024];
	while (true)
	{
		//header 리드
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
		else //패킷 파싱
		{
			//패킷 종류
			char Method = Header[0];
			//패킷 길이
			char Length = Header[1];

			//패킷 바디 읽어 오기
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
				//최초 접속 후 정보 처리
				case MSGType::LoginAck:
					memcpy(&id, &Buffer[0], 4);
					PlayerList[0].UserID = id; //자기 아이디를 입력
					break;
				//다른 유저 생성
				case MSGType::MakeUser:
					memcpy(&x, &Buffer[0], 4);
					memcpy(&y, &Buffer[4], 4);
					memcpy(&id, &Buffer[8], 4);

					//모든 유저 리스트가 오기 때문에
					//이미 생성된 유저는 생성하지 않음
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

						//동기화 시작
						EnterCriticalSection(&cs);
						PlayerList.push_back(NewPlayer);
						//동기화 끝
						LeaveCriticalSection(&cs);
		
						Utility::Log(string("다른 클라이언트 신규 유저 추가"));
					}
					break;
				case MSGType::DeleteUser:
				{
					Utility::Log(string("다른 유저 퇴장"));

					memcpy(&id, &Buffer[0], 4);
					auto it = PlayerList.begin();
					while (it != PlayerList.end())
					{
						if (it->UserID == id)
						{
							//동기화 시작
							EnterCriticalSection(&cs);
							it = PlayerList.erase(it);
							//동기화 끝
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
							//동기화 시작
							EnterCriticalSection(&cs);
							it->x = x;
							it->y = y;
							//동기화 끝
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
