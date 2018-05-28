// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/PlayerCameraManager.h"
#include "MyPlayerCameraManager.generated.h"

/**
 * 
 */
UCLASS()
class DAY04_API AMyPlayerCameraManager : public APlayerCameraManager
{
	GENERATED_BODY()
	
	
	
public:
	AMyPlayerCameraManager();

	virtual void BeginPlay() override;
	virtual void UpdateCamera(float DeltaTime) override;

	float CrouchOffset;
	float DefaultCameraZ;

	float NormalFOV = 90.0f; // �Ϲ�
	float IronsightFOV = 60.0f; // ���ؽ�

	FVector SpringArmOffset;
	float DefaultFOV; //�����Ӻ��� �ٲ�� FOV

};
