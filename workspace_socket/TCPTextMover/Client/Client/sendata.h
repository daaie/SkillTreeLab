#pragma once 
#include <WinSock2.h>
#include "msgtype.h"

class SendData
{
public:
	static char Data[1024];


	static void Move(SOCKET receiver, int x, int y, int id)
	{
		Data[0] = (char)MSGType::Move;
		Data[1] = 12;
		memcpy(&Data[2], &x, sizeof(x));
		memcpy(&Data[6], &y, sizeof(y));
		memcpy(&Data[10], &id, sizeof(id));

		send(receiver, Data, 14, 0);
	}

	static void LoginAck(SOCKET receiver, int id)
	{
		Data[0] = (char)MSGType::LoginAck;
		Data[1] = 4;
		memcpy(&Data[2], &id, sizeof(id));

		send(receiver, Data, 6, 0);
	}

	static void MakeUser(SOCKET receiver, int x, int y, int id)
	{
		Data[0] = (char)MSGType::MakeUser;
		Data[1] = 12;
		memcpy(&Data[2], &x, sizeof(x));
		memcpy(&Data[6], &y, sizeof(y));
		memcpy(&Data[10], &id, sizeof(id));

		send(receiver, Data, 14, 0);
	}

	static void DeleteUser(SOCKET receiver, int id)
	{
		Data[0] = (char)MSGType::DeleteUser;
		Data[1] = 4;
		memcpy(&Data[2], &id, sizeof(id));

		send(receiver, Data, 6, 0);
	}
};

char SendData::Data[1024];