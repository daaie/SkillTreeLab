// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_SelectPatrolPoint.generated.h"

/**
 * 
 */
UCLASS()
class DAY04_API UBTTask_SelectPatrolPoint : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	
public:	

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	// �������̵� �����. �ٷ� �θ𿡼� �������̵��ؼ� �����Ѱ� �ƴ϶� ���� �����Ѱű� ������ ó�� �����ϴ°���. 
};
