#pragma once
#include "DXApp.h"

class Engine : public DXApp
{
public:
	Engine(HINSTANCE hinstance);
	~Engine(); //구현단계...가상 소멸자 필요없음.

	bool Init() override;
	void Update() override;
	void Render() override;
};

