// Fill out your copyright notice in the Description page of Project Settings.

#include "ChangeSpeed.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "Zombie/MyZombie.h"
#include "AIController.h"
#include "GameFramework/CharacterMovementComponent.h"

EBTNodeResult::Type UChangeSpeed::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AMyZombie* Pawn = Cast<AMyZombie>(OwnerComp.GetAIOwner()->GetPawn());

	if (Pawn)
	{
		switch (Pawn->CurrentAnimState)
		{
			case EZombieAnimState::Walk:
			{
				Pawn->GetCharacterMovement()->MaxWalkSpeed = Pawn->WalkSpeed;
			}
			break;

			case EZombieAnimState::Run:
			{
				Pawn->GetCharacterMovement()->MaxWalkSpeed = Pawn->RunSpeed;
			}
			break;

		}

		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}


