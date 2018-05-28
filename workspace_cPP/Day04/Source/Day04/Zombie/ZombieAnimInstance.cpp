// Fill out your copyright notice in the Description page of Project Settings.

#include "ZombieAnimInstance.h"
#include "CharacterZombie.h"
#include "GameFramework/CharacterMovementComponent.h"

UZombieAnimInstance::UZombieAnimInstance()
{
}


void UZombieAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	ACharacterZombie *Pawn = Cast<ACharacterZombie>(TryGetPawnOwner());

	if (Pawn && Pawn->IsValidLowLevel())
	{
		CurrentAnimState = Pawn->CurrentAnimState;
		CurrentState = Pawn->CurrentState;
	}
}