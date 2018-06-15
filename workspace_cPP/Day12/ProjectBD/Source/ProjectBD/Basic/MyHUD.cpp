// Fill out your copyright notice in the Description page of Project Settings.

#include "MyHUD.h"
#include "ConstructorHelpers.h"
#include "Engine/Canvas.h"
#include "Engine/Texture2D.h"


AMyHUD::AMyHUD()
{
	static ConstructorHelpers::FObjectFinder<UTexture2D> T_Crosshair(TEXT("Texture2D'/Game/Weapons/crosshair.crosshair'"));
	if (T_Crosshair.Succeeded())
	{
		Crosshair = T_Crosshair.Object;
	}
}

void AMyHUD::DrawHUD()
{
	Super::DrawHUD();

	if (Crosshair)
	{
		DrawTextureSimple(Crosshair,
			Canvas->SizeX / 2 - Crosshair->GetSizeX() / 2,
			Canvas->SizeY / 2 - Crosshair->GetSizeY() / 2);
	}
}
