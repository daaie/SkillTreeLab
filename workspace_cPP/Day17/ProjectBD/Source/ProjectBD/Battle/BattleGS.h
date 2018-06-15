// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "BattleGS.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTBD_API ABattleGS : public AGameStateBase
{
	GENERATED_BODY()

public:	
	ABattleGS();

	virtual void BeginPlay() override;

	UFUNCTION(NetMulticast, Reliable)
	void S2A_AddBattleInfo(const FString& Message);
	void S2A_AddBattleInfo_Implementation(const FString& Message);

	void TimerFunction();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, ReplicatedUsing = OnRep_AliveCount)
	int AliveCount;

	UFUNCTION()
	void OnRep_AliveCount();


	UPROPERTY(BlueprintReadWrite, EditAnywhere, ReplicatedUsing = OnRep_PublicMessage)
	FString PublicMessage;

	UFUNCTION()
	void OnRep_PublicMessage(); //���� ���ø����̼�(��Ʈ��ũ)�� ���� �Ǿ������� ����

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;
};
