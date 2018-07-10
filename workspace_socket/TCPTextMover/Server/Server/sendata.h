#pragma once 
#include <WinSock2.h>
#include "msgtype.h"

/////////////////////////////
// 패킷을 생성 해서 발송
/////////////////////////////
class SendData
{
public:
	//패킷 버퍼
	static char Data[1024];

	//이동 패킷
	static void Move(SOCKET receiver, int x, int y, int id)
	{
		Data[0] = (char)MSGType::Move;
		Data[1] = 12;
		memcpy(&Data[2], &x, sizeof(x));
		memcpy(&Data[6], &y, sizeof(y));
		memcpy(&Data[10], &id, sizeof(id));

		send(receiver, Data, 14, 0);
	}

	//연결 후 플레이어 정보(서버 할당 아이디를 전송)
	static void LoginAck(SOCKET receiver, int id)
	{
		Data[0] = (char)MSGType::LoginAck;
		Data[1] = 4;
		memcpy(&Data[2], &id, sizeof(id));

		send(receiver, Data, 6, 0);
	}

	//다른 클라이언트를 생성
	static void MakeUser(SOCKET receiver, int x, int y, int id)
	{
		Data[0] = (char)MSGType::MakeUser;
		Data[1] = 12;
		memcpy(&Data[2], &x, sizeof(x));
		memcpy(&Data[6], &y, sizeof(y));
		memcpy(&Data[10], &id, sizeof(id));

		send(receiver, Data, 14, 0);
	}

	//종료된 클라이언트 삭제
	static void DeleteUser(SOCKET receiver, int id)
	{
		Data[0] = (char)MSGType::DeleteUser;
		Data[1] = 4;
		memcpy(&Data[2], &id, sizeof(id));

		send(receiver, Data, 6, 0);
	}
};

char SendData::Data[1024];