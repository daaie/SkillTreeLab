// Fill out your copyright notice in the Description page of Project Settings.

#include "MyHUD.h"
#include "ConstructorHelpers.h"
#include "Engine/Canvas.h" // 전체화면 사이즈.
#include "Engine/Texture2D.h"

AMyHUD::AMyHUD()
{
	static ConstructorHelpers::FObjectFinder<UTexture2D> T_Crosshair(TEXT("Texture2D'/Game/Weapons/crosshair.crosshair'"));
	if (T_Crosshair.Succeeded())
	{
		Crosshair=T_Crosshair.Object;
	}

}

void AMyHUD::DrawHUD()
{
	Super::DrawHUD();

	if (Crosshair)
	{
		// DrawTexture도있음
		DrawTextureSimple(Crosshair, (Canvas->SizeX - Crosshair->GetSizeX())/2, (Canvas->SizeY - Crosshair->GetSizeY()) /2);
	}

}
