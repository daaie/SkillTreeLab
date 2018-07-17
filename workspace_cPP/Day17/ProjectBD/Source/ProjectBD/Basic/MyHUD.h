// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "MyHUD.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTBD_API AMyHUD : public AHUD
{
	GENERATED_BODY()

public:
	AMyHUD();
	
	virtual void DrawHUD() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UTexture2D* Crosshair;

	//���̴�
	void DrawRadarBG();
	void DrawPlayer();
	void FindRadarObject();
	void DrawRadarObject();

	// ���̴� ������Ʈ ������
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float DrawRadarObjectSize = 5;

	// ȭ�� �� ���̴� (��ġ ������ -> ������ �Ʒ��� ��ǥ ����)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector2D RadarPosition = FVector2D(0.9f, 0.3f);

	// ���̴� ũ��
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float RadarRadius = 100.0f;

	// ���̴� ��� ��ǥ
	FVector2D GetRadarCenterPosition();

	// ���̴��� �׸� ��ü ����Ʈ
	TArray<class AActor*> RadarObjectList;

	// ���̴� �׸� ��ü �±� 
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString RadarObjectTagName;

	// ���̴� ��� ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float RadarRatio = 10.0f;

	// ���ΰ� ���� ���� ��ǥ��� ��ȯ , ���̴� ��� �������� ����. 3D->2D��..
	
	FVector2D ConverWorldToPlayerLocal(class AActor* RadarActor);


};
