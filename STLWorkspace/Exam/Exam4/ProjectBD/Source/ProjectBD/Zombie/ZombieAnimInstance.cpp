// Fill out your copyright notice in the Description page of Project Settings.

#include "ZombieAnimInstance.h"
#include "ZombieCharacter.h"

void UZombieAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	AZombieCharacter* Pawn = Cast<AZombieCharacter>(TryGetPawnOwner());
	if(Pawn&&Pawn->IsValidLowLevel())
	{
		CurrentAnimState = Pawn->CurrentAnimState;
		CurrentState = Pawn->CurrentState;
	}
}

void UZombieAnimInstance::AnimNotify_StartAttack(UAnimNotify * Notify)
{
	UE_LOG(LogClass, Warning, TEXT("승주 바보"));
	AZombieCharacter* Pawn = Cast<AZombieCharacter>(TryGetPawnOwner());
	if (Pawn&&Pawn->IsValidLowLevel())
	{
		Pawn->OnAttack();
	}
}
