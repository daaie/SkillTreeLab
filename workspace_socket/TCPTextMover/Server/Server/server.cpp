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

//플레이어 관리 리스트
vector<Player> PlayerList;
//map<SOCKET, Player> PlayerListMap;

int main()
{
	//윈속 객체 데이터
	WSAData WsaData;
	//클라이언트 소켓 번호
	SOCKET ClientSocket;
	//서버 소켓 번호
	SOCKET ServerSocket;
	//클라이언트 주소 정보
	SOCKADDR_IN ClientAddr;
	//서버 주소 정보
	SOCKADDR_IN ServerAddr;

	//select 함수 타임 아웃 시간 구조체
	TIMEVAL Timeout;

	//멀티플렉싱을 위한 구조체
	fd_set Reads;
	fd_set CopyReads;

	//주소 구조체 길이용 변수
	int AddressSize;
	//자료 임시 저장용
	char Buffer[1024];

	//패킷 헤더용
	char Header[2];


	//윈속 초기화
	if (WSAStartup(MAKEWORD(2, 2), &WsaData) != 0)
	{
		printf("Winsock2 Error\n");
		exit(-1);
	}

	//서버소켓 만들기
	ServerSocket = socket(PF_INET, SOCK_STREAM, 0);
	//서버 주소 구조체 세팅
	memset(&ServerAddr, 0, sizeof(ServerAddr));
	ServerAddr.sin_family = AF_INET;
	ServerAddr.sin_addr.s_addr = htonl(INADDR_ANY); //서버 아무 주소나 씀, 랜카드 여러개면 정확히 해야됨
	ServerAddr.sin_port = htons(3000);

	//소켓 바인딩
	if (bind(ServerSocket, (SOCKADDR*)&ServerAddr, sizeof(ServerAddr)) == SOCKET_ERROR)
	{
		printf("Socket bind error\n");
		exit(-1);
	}

	//서버 포트에서 듣기 설정
	if (listen(ServerSocket, 5) == SOCKET_ERROR)
	{
		printf("Socket listen error\n");
		exit(-1);
	}


	//입출력 알림을 위한 구조체 초기화
	FD_ZERO(&Reads);

	//입출력 알림을 위한 구조체를 OS 등록, 서버 소켓에 연결
	FD_SET(ServerSocket, &Reads);

	int fdNum = 0;

	Utility::Log(string("서버 시작"));

	while (true)
	{
		CopyReads = Reads;

		//소켓 타임 아웃 시간
		Timeout.tv_sec = 10;
		Timeout.tv_usec = 0;

		//Socket에서 Read만 감시
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
			//상태 변화 감지
			if (FD_ISSET(Reads.fd_array[i], &CopyReads))
			{
				if (Reads.fd_array[i] == ServerSocket) //클라이언트가 연결 요청을 함
				{
					AddressSize = sizeof(ClientAddr);
					ClientSocket = accept(ServerSocket,
						(SOCKADDR*)&ClientAddr, &AddressSize);
					//연결된 클라이언트 소켓을 입출력 감지에 등록
					FD_SET(ClientSocket, &Reads);
					printf("Client connect %d\n", ClientSocket);


					Utility::Log(string("신규 유저 입장"));

					//Send LoginAck
					//들어온 유저한테 ID 발급
					SendData::LoginAck(ClientSocket, ClientSocket);
					
					//리스트에 신규 유저 추가
					Player NewPlayer;
					NewPlayer.x = 0;
					NewPlayer.y = 0;
					NewPlayer.UserID = ClientSocket;
					NewPlayer.ClientSocket = ClientSocket;
					PlayerList.push_back(NewPlayer);

					//기존에 있던 유저 생성하기
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
				else //서버가 정보를 받음, recv처리
				{
					//버퍼 초기화
					memset(Buffer, 0, 1024);

					//헤더를 읽음
					int RecvLen = recv(Reads.fd_array[i], Header, 2, 0);
					//int Length = recv(Reads.fd_array[i], Buffer, 1024 - 1, 0);
					if (RecvLen == 0 || RecvLen == SOCKET_ERROR) //연결 종료
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

						//소켓 감지 리스트에서 제거
						FD_CLR(Reads.fd_array[i]/*종료된 클라이언트 소켓*/, &Reads);
						
						closesocket(CopyReads.fd_array[i]);
						printf("Client Close %d\n", CopyReads.fd_array[i]);
					}
					else //메세지 들어온거
					{
						//헤더 처리
						//int RecvLen = recv(Reads.fd_array[i], Header, 2, 0);
						//메세지 종류
						char Method = Header[0];
						//메세지 길이
						char Length = Header[1];

						//메세지 바디 읽어 오기
						int BodyRecvLen = 0;
						RecvLen = 0;
						while (BodyRecvLen < Length) //TCP는 한번에 못 받아 올수도 있어서 여러번 읽어서 패킷을 읽음.
						{
							RecvLen = recv(Reads.fd_array[i], &Buffer[BodyRecvLen], Length, 0);
							if (RecvLen == 0 || RecvLen == SOCKET_ERROR)
							{
								Utility::Log(string("유저 퇴장"));

								closesocket(Reads.fd_array[i]);
								return 0;
							}
							else
							{
								BodyRecvLen += RecvLen;
							}
						}
						
						//메세지 처리
						switch ((MSGType)Method)
						{
							//이동
							case MSGType::Move:
							{
								Utility::Log(string("유저 이동"));
								int x;
								int y;
								int id;

								//이동 패킷 파싱
								memcpy(&x, &Buffer[0], 4);
								memcpy(&y, &Buffer[4], 4);
								memcpy(&id, &Buffer[8], 4);
								
								//서버 정보 업데이트
								for (auto it = PlayerList.begin(); it != PlayerList.end(); ++it)
								{
									if (it->UserID == id)
									{
										it->x = x;
										it->y = y;
									}
								}

								//업데이트 된 정보를 다른 유저한테 전송
								for (auto it : PlayerList)
								{
									if (it.UserID != id) //자기 자신한테는 보내지 않는다.
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

	//서버 소켓 종료, 윈속 라이브러리 정리
	closesocket(ServerSocket);
	WSACleanup();

	return 0;
}