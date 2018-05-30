// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_SetDestination.h"
#include "ZombieCharacter.h"
#include "ZombieAIController.h"
#include "ZombieTargetPoint.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type UBTTask_SetDestination::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AZombieCharacter* Pawn = Cast<AZombieCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	if (Pawn && Pawn->IsValidLowLevel())
	{
		AZombieTargetPoint* Target = Pawn->Targets[FMath::RandRange(0,Pawn->Targets.Num()-1)];
		OwnerComp.GetBlackboardComponent()->SetValueAsObject(GetSelectedBlackboardKey(), Target);

		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}

