// Fill out your copyright notice in the Description page of Project Settings.

#include "CheckBattle.h"
#include "Zombie/MyZombie.h"
#include "Zombie/ZombieAIController.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/PawnSensingComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Basic/MyCharacter.h"

EBTNodeResult::Type UCheckBattle::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	AMyZombie* Zombie = Cast<AMyZombie>(OwnerComp.GetAIOwner()->GetPawn());

	AMyCharacter* Player = Cast<AMyCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(FName(TEXT("Target"))));
	float Range = FVector::Distance(Zombie->GetActorLocation(), Player->GetActorLocation());
	FVector Dir = Player->GetActorLocation() - Zombie->GetActorLocation();
	Zombie->SetActorRotation(Dir.Rotation());
	if (Range > Zombie->AttackRange)
	{
		Zombie->CurrentState = EZombieState::Chase;
		OwnerComp.GetBlackboardComponent()->SetValueAsEnum(FName(TEXT("CurrentState")), (uint8)Zombie->CurrentState);

		return EBTNodeResult::Failed;
	}
	else if (Player->CurrentHP <= 0)
	{
		Zombie->CurrentState = EZombieState::Normal;
		OwnerComp.GetBlackboardComponent()->SetValueAsEnum(FName(TEXT("CurrentState")), (uint8)Zombie->CurrentState);

		return EBTNodeResult::Failed;
	}

	return EBTNodeResult::Succeeded;
}
