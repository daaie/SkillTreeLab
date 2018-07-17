// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/PlayerCameraManager.h"
#include "BasicPCM.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTBD_API ABasicPCM : public APlayerCameraManager
{
	GENERATED_BODY()
	
public:
	ABasicPCM();
	
	virtual void BeginPlay() override;

	virtual void UpdateCamera(float DeltaTime) override;

	FVector SpringArmOffset; // 스프링암 높이 오프셋

	float DefaultFOV;	//프레이별로 바뀌는 FOV

	float NormalFOV = 90.0f;		//일반
	float IronsightFOV = 60.0f;		//조준시 
	
};
