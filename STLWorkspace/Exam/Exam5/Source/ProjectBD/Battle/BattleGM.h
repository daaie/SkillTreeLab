// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "BattleGM.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTBD_API ABattleGM : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;
	

	TArray<AActor*> ItemPoints;
	

	UFUNCTION(Server, WithValidation, Reliable)
	void C2S_CheckAliveCount();
	bool C2S_CheckAliveCount_Validate();
	void C2S_CheckAliveCount_Implementation();


	FTimerHandle TimerHandle;

	UFUNCTION()
	void ReturnLobby(); //�κ�� ���ư��� Ÿ�̸� �Լ�

	UFUNCTION()
	void TimerFunction();

	int LeftTime = 10;

};

