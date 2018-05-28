// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_SetWaitTime.h"

#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "AIController.h"

EBTNodeResult::Type UBTTask_SetWaitTime::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	OwnerComp.GetBlackboardComponent()->SetValueAsFloat(GetSelectedBlackboardKey(), FMath::FRandRange(MinTime, MaxTime));

	return EBTNodeResult::Succeeded;
}