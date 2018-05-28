// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_SetAnimStateWalk.h"

#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "AIController.h"

#include "CharacterZombie.h"

EBTNodeResult::Type UBTTask_SetAnimStateWalk::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ACharacterZombie* Pawn = Cast<ACharacterZombie>(OwnerComp.GetAIOwner()->GetPawn());

	if (Pawn)
	{
		Pawn->CurrentAnimState = EZombieAnimState::Walk;
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}
