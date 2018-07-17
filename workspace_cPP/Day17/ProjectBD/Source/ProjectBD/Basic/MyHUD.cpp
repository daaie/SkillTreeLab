// Fill out your copyright notice in the Description page of Project Settings.

#include "MyHUD.h"
#include "ConstructorHelpers.h"
#include "Engine/Canvas.h"
#include "Engine/Texture2D.h"
#include "Kismet/GameplayStatics.h"
#include "Item/MasterItem.h"

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
	
	DrawRadarBG();

	DrawPlayer();
	FindRadarObject();
	DrawRadarObject();
}

void AMyHUD::DrawRadarBG()
{
	FVector2D RadarCenter = GetRadarCenterPosition();

	for (int i = 0; i < 360; ++i)
	{
		float X = FMath::Cos(i)*RadarRadius;
		float Y = FMath::Sin(i)*RadarRadius;

		DrawLine(RadarCenter.X, RadarCenter.Y, RadarCenter.X+X, RadarCenter.Y+Y, FLinearColor::Gray, 3.0f);
	}
}

void AMyHUD::DrawPlayer()
{
	FVector2D RadarCenter = GetRadarCenterPosition();

	DrawRect(FLinearColor::Red, RadarCenter.X, RadarCenter.Y, DrawRadarObjectSize, DrawRadarObjectSize);


}

void AMyHUD::FindRadarObject()
{
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AMasterItem::StaticClass(), RadarObjectList);
	//UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName(*RadarObjectTagName), RadarObjectList);

}

FVector2D AMyHUD::ConverWorldToPlayerLocal(AActor * RadarActor)
{
	APawn* MyPlayer = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if (MyPlayer && RadarActor)
	{
		FVector ActorLocal3DPosition = MyPlayer->GetTransform().InverseTransformPosition(RadarActor->GetActorLocation());
		// w주인공 기준으로 월드로케이션 2D로 바꿔줌

		ActorLocal3DPosition = FRotator(0, -90, 0).RotateVector(ActorLocal3DPosition);
		ActorLocal3DPosition /= 10.0f;

		return FVector2D(ActorLocal3DPosition);
	}
	return FVector2D();
}

void AMyHUD::DrawRadarObject()
{
	APawn* MyPlayer = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	FVector2D RadarCenter = GetRadarCenterPosition();

	for (auto it : RadarObjectList)
	{
		FVector2D ConvertLocation = ConverWorldToPlayerLocal(it);
		FVector Temp = FVector(ConvertLocation.X, ConvertLocation.Y, 0);
		Temp = Temp.GetClampedToMaxSize2D(RadarRadius - DrawRadarObjectSize);

		DrawRect(FLinearColor::Blue, 
			RadarCenter.X + Temp.X,
			RadarCenter.Y + Temp.Y,
			DrawRadarObjectSize, 
			DrawRadarObjectSize);
	}
}

FVector2D AMyHUD::GetRadarCenterPosition()
{
	// 허드에서 그리는 영역 -> Canvas
	if (Canvas)
	{
		return FVector2D(Canvas->SizeX * RadarPosition.X, Canvas->SizeY*RadarPosition.Y);
	}

	return FVector2D();
}

