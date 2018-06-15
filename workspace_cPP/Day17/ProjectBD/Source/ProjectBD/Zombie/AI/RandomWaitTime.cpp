// Fill out your copyright notice in the Description page of Project Settings.

#include "RandomWaitTime.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type URandomWaitTime::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	OwnerComp.GetBlackboardComponent()->SetValueAsFloat(GetSelectedBlackboardKey(), FMath::FRandRange(MinTime, MaxTime));

	return EBTNodeResult::Succeeded;
}
