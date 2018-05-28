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
	// 오버라이드 없어도됨. 바로 부모에서 오버라이드해서 구현한게 아니라 새로 선언한거기 떄문에 처음 구현하는거임. 
};
