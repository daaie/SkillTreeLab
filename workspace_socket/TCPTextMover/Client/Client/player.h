#pragma once

#include "utility.h"

#define UP 0x48
#define LEFT 0x4B
#define RIGHT 0x4D
#define DOWN 0x50
#define SPACE 0x20

//////////////////////////////
// �÷��̾� ó�� 
/////////////////////////////
class Player
{
public:
	//���� ��ġ
	int x;
	//���� ��ġ
	int y;

	//���� ���̵�
	int UserID;
	//Ŭ���̾�Ʈ ���� 
	SOCKET ClientSocket;

	//�̵� ó��
	void Move(int KeyCode);
};

//�̵� ó�� �Լ�
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