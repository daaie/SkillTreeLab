// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_CheckBattle.h"
#include "Basic/MyCharacter.h"
#include "ZombieCharacter.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "ZombieAIController.h"

EBTNodeResult::Type UBTTask_CheckBattle::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AMyCharacter* Player = Cast<AMyCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(FName(TEXT("Target"))));
	AZombieCharacter* Zombie = Cast<AZombieCharacter>(OwnerComp.GetAIOwner()->GetPawn());

	if (Player && Zombie)
	{

		FVector Dir = Player->GetActorLocation() - Zombie->GetActorLocation();
		Zombie->SetActorRotation(Dir.Rotation());
		float Range = FVector::Distance(Zombie->GetActorLocation(), Player->GetActorLocation());

		if (Range > Zombie->AttackRange)
		{
			Zombie->CurrentState = EZombieState::CHASE;
			Zombie->CurrentAnimState = EZombieAnimState::Run;
			OwnerComp.GetBlackboardComponent()->SetValueAsEnum(FName(TEXT("CurrentState")), (uint8)Zombie->CurrentState);

			return EBTNodeResult::Failed;
		}
		else if (Player->CurrentHP <= 0)
		{
			Zombie->CurrentState = EZombieState::NORMAL;
			OwnerComp.GetBlackboardComponent()->SetValueAsEnum(FName(TEXT("CurrentState")), (uint8)Zombie->CurrentState);

			return EBTNodeResult::Failed;
		}
	}

	return EBTNodeResult::Succeeded;
}