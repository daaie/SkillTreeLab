// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_SelectPatrolPoint.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "AIController.h"
#include "Zombie/MyZombie.h"
#include "Zombie/PatrolPoint.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type UBTTask_SelectPatrolPoint::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	AMyZombie* Pawn = Cast<AMyZombie>(OwnerComp.GetAIOwner()->GetPawn());

	if (Pawn)
	{
		APatrolPoint* Point = Pawn->PatrolPoints[FMath::RandRange(0, Pawn->PatrolPoints.Num() - 1)];

		OwnerComp.GetBlackboardComponent()->SetValueAsObject(GetSelectedBlackboardKey(), Point);

		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}
