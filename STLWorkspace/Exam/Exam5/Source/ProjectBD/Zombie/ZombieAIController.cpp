// Fill out your copyright notice in the Description page of Project Settings.

#include "ZombieAIController.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Zombie/MyZombie.h"
#include "BehaviorTree/BehaviorTree.h"

AZombieAIController::AZombieAIController()
{
	BTComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorTree"));
	BBComponent = CreateDefaultSubobject<UBlackboardComponent>(TEXT("Blackboard"));
}

void AZombieAIController::Possess(APawn * InPawn)
{
	Super::Possess(InPawn);

	AMyZombie* Zombie = Cast<AMyZombie>(InPawn);

	if (Zombie)
	{
		BBComponent->InitializeBlackboard(*(Zombie->BehaviorTree->BlackboardAsset));
		BTComponent->StartTree(*(Zombie->BehaviorTree));
	}
}
