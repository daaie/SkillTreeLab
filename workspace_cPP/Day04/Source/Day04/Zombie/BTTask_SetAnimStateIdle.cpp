// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_SetAnimStateIdle.h"

#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "AIController.h"

#include "CharacterZombie.h"

EBTNodeResult::Type UBTTask_SetAnimStateIdle::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ACharacterZombie* Pawn = Cast<ACharacterZombie>(OwnerComp.GetAIOwner()->GetPawn());

	if (Pawn)
	{
		Pawn->CurrentState = NewCurrentState;
		Pawn->CurrentAnimState = NewCurrentAnimState;
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}