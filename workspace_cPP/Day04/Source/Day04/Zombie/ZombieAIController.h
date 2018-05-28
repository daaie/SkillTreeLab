// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "ZombieAIController.generated.h"

/**
 * 
 */
UCLASS()
class DAY04_API AZombieAIController : public AAIController
{
	GENERATED_BODY()
	
	
public:
	AZombieAIController();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class UBehaviorTreeComponent * BTComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class UBlackboardComponent * BBComponent;
	
	virtual void Possess(APawn* InPawn) override; // 얘는 AIController의 부모로부터 오버라이드 한거라서 수퍼해줘야 작동한다

};
