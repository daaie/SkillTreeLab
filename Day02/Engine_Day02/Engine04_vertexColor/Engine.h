#pragma once
#include "DXApp.h"

class Engine : public DXApp
{
public:
	Engine(HINSTANCE hinstance);
	~Engine(); //�����ܰ�...���� �Ҹ��� �ʿ����.

	bool Init() override;
	void Update() override;
	void Render() override;
};

