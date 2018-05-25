#pragma once
#include "DXApp.h"

class Engine : public DXApp
{
public:
	Engine(HINSTANCE hinstance);
	~Engine();

	bool Init() override;
	void Update(double deltaTime) override;
	void Render() override;

	void RotateObject(double deltaTime);
};