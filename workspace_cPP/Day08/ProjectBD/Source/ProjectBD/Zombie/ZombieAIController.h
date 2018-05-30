// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "ZombieCharacter.h"
#include "ZombieAIController.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTBD_API AZombieAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	AZombieAIController();
	virtual void Possess(APawn* InPawn) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UBehaviorTreeComponent* BTComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UBlackboardComponent* BBComponent;
};
