// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_SetZombieAnimState.h"
#include "ZombieCharacter.h"
#include "ZombieAIController.h"
#include "BehaviorTree/BlackboardComponent.h"


EBTNodeResult::Type UBTTask_SetZombieAnimState::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AZombieCharacter* Pawn = Cast<AZombieCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	if (Pawn)
	{
		Pawn->CurrentAnimState = CurrentAnimState;
		Pawn->CurrentState = CurrentState;

		OwnerComp.GetBlackboardComponent()->SetValueAsEnum(GetSelectedBlackboardKey(), (uint8)CurrentState);

		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}

