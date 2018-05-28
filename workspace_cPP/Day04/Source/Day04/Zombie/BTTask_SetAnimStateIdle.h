// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "CharacterZombie.h"
#include "BTTask_SetAnimStateIdle.generated.h"

/**
 * 
 */
UCLASS()
class DAY04_API UBTTask_SetAnimStateIdle : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	
	
	
public:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
		EZombieState NewCurrentState;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
		EZombieAnimState NewCurrentAnimState;
};
