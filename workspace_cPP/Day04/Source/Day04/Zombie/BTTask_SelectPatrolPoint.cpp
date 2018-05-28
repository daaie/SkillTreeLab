// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_SelectPatrolPoint.h"

#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "AIController.h"

#include "CharacterZombie.h"
#include "MyTargetPoint.h"

EBTNodeResult::Type UBTTask_SelectPatrolPoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ACharacterZombie* Pawn = Cast<ACharacterZombie>(OwnerComp.GetAIOwner()->GetPawn());

	if (Pawn)
	{
		AMyTargetPoint* Points = Pawn->PatrolPoints[FMath::RandRange(0,Pawn->PatrolPoints.Num()-1)];
		OwnerComp.GetBlackboardComponent()->SetValueAsObject(GetSelectedBlackboardKey(), Points);

		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}