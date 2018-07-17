// Fill out your copyright notice in the Description page of Project Settings.

#include "ZombieAnimInstance.h"

void UZombieAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	AMyZombie* Pawn = Cast<AMyZombie>(TryGetPawnOwner());
	if (Pawn && Pawn->IsValidLowLevelFast())
	{
		CurrentAnimState = Pawn->CurrentAnimState;
		CurrentState = Pawn->CurrentState;
	}
}

void UZombieAnimInstance::AnimNotify_OnAttack(UAnimNotify * Notify)
{
	AMyZombie* Pawn = Cast<AMyZombie>(TryGetPawnOwner());
	if (Pawn && Pawn->IsValidLowLevelFast())
	{
		Pawn->OnAttack();
	}
}
