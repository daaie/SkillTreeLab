// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_SetSpeed.h"
#include "ZombieCharacter.h"
#include "AIController.h"
#include "GameFramework/CharacterMovementComponent.h"

EBTNodeResult::Type UBTTask_SetSpeed::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AZombieCharacter* Pawn = Cast<AZombieCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	if (Pawn && Pawn->IsValidLowLevel())
	{
		switch (Pawn->CurrentAnimState)
		{
		case EZombieAnimState::Walk :
		{ // 괄호하면 변수 선언가능.
			Pawn->GetCharacterMovement()->MaxWalkSpeed = Pawn->WalkSpeed;
		}
			break;
		case EZombieAnimState::Run:
		{
			Pawn->GetCharacterMovement()->MaxWalkSpeed = Pawn->RunSpeed;
		}
			break;
		default:
			Pawn->GetCharacterMovement()->MaxWalkSpeed = Pawn->WalkSpeed;
			break;
		}
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}
