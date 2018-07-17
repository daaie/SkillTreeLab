// Fill out your copyright notice in the Description page of Project Settings.

#include "MyAnimInstance.h"
#include "Basic/MyCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

void UMyAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	AMyCharacter* Pawn = Cast<AMyCharacter>(TryGetPawnOwner());
	if (Pawn && Pawn->IsValidLowLevel())
	{
		Speed = Pawn->GetCharacterMovement()->Velocity.Size();
		Direction = CalculateDirection(Pawn->GetCharacterMovement()->Velocity, Pawn->GetActorRotation());

		bIsCrouch = Pawn->bIsCrouched;

		AimYaw = Pawn->GetAimOffset().Yaw;
		AimPitch = Pawn->GetAimOffset().Pitch;
		bIsIronsight = Pawn->bIsIronsight;
		bIsSprint = Pawn->bIsSprint;
		bIsProne = Pawn->bIsProne;
		bIsProning = Pawn->bIsProning;
		bIsFire = Pawn->bIsFire;
	}
}

void UMyAnimInstance::AnimNotify_ProneStart(UAnimNotify * Notify)
{
	AMyCharacter* Pawn = Cast<AMyCharacter>(TryGetPawnOwner());
	if (Pawn && Pawn->IsValidLowLevel())
	{
		Pawn->bIsProning = true;
	}
}

void UMyAnimInstance::AnimNotify_ProneEnd(UAnimNotify * Notify)
{
	AMyCharacter* Pawn = Cast<AMyCharacter>(TryGetPawnOwner());
	if (Pawn && Pawn->IsValidLowLevel())
	{
		Pawn->bIsProning = false;
	}
}


