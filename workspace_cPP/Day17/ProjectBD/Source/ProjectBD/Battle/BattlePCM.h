// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/PlayerCameraManager.h"
#include "BattlePCM.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTBD_API ABattlePCM : public APlayerCameraManager
{
	GENERATED_BODY()
public:
	ABattlePCM();

	virtual void BeginPlay() override;

	virtual void UpdateCamera(float DeltaTime) override;

	FVector SpringArmOffset; // �������� ���� ������

	float DefaultFOV;	//�����̺��� �ٲ�� FOV

	float NormalFOV = 90.0f;		//�Ϲ�
	float IronsightFOV = 60.0f;		//���ؽ� 
	
	
	
};
