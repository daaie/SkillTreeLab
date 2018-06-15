// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "Zombie/MyZombie.h"
#include "SetZombieState.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTBD_API USetZombieState : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
	UPROPERTY(EditAnywhere, Category = Blackboard)
	EZombieState NewZombieState;

	UPROPERTY(EditAnywhere, Category = Blackboard)
	EZombieAnimState NewZombieAnimState;
	
};
