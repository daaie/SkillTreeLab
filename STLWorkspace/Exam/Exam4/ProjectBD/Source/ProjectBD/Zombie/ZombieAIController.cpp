// Fill out your copyright notice in the Description page of Project Settings.

#include "ZombieAIController.h"
#include "ZombieCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BehaviorTree.h"

AZombieAIController::AZombieAIController()
{
	BTComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BTComponent"));
	BBComponent = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BBComponent"));
}

void AZombieAIController::Possess(APawn * InPawn)
{
	Super::Possess(InPawn);

	AZombieCharacter * Zombie = Cast<AZombieCharacter>(InPawn);

	if (Zombie)
	{
		BBComponent->InitializeBlackboard(*(Zombie->BehaviorTree->BlackboardAsset));
		BTComponent->StartTree(*(Zombie->BehaviorTree));
		
	}
}
