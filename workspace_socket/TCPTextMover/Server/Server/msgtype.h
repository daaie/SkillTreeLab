#pragma once

///////////////////////////////
// 네트워크 패킷 종류
///////////////////////////////
enum class MSGType
{
	Login = 10,
	LoginAck = 11,
	Move = 20,
	MakeUser = 30,
	DeleteUser = 40
};