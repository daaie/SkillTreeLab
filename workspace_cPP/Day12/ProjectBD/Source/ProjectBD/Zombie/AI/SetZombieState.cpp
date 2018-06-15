// Fill out your copyright notice in the Description page of Project Settings.

#include "SetZombieState.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "AIController.h"
#include "Zombie/MyZombie.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type USetZombieState::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	AMyZombie* Pawn = Cast<AMyZombie>(OwnerComp.GetAIOwner()->GetPawn());

	if (Pawn)
	{
		Pawn->CurrentState = NewZombieState;
		Pawn->CurrentAnimState = NewZombieAnimState;

		OwnerComp.GetBlackboardComponent()->SetValueAsEnum(GetSelectedBlackboardKey(), (uint8)NewZombieState);

		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}
