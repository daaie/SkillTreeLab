#pragma once

#include "utility.h"

#define UP 0x48
#define LEFT 0x4B
#define RIGHT 0x4D
#define DOWN 0x50
#define SPACE 0x20

//////////////////////////////
// 플레이어 처리 
/////////////////////////////
class Player
{
public:
	//가로 위치
	int x;
	//세로 위치
	int y;

	//유저 아이디
	int UserID;
	//클라이언트 소켓 
	SOCKET ClientSocket;

	//이동 처리
	void Move(int KeyCode);
};

//이동 처리 함수
void Player::Move(int KeyCode)
{
	switch (KeyCode)
	{
	case UP:
		y--;
		break;
	case DOWN:
		y++;
		break;
	case LEFT:
		x--;
		break;
	case RIGHT:
		x++;
		break;
	}

	Utility::clamp(x, 0, 80);
	Utility::clamp(y, 0, 25);
}