// Fill out your copyright notice in the Description page of Project Settings.

#include "AnimNotifyState_ZombieMove.h"
#include "ZombieCharacter.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

void UAnimNotifyState_ZombieMove::NotifyTick(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float FrameDeltaTime)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime);

	AZombieCharacter* Pawn = Cast<AZombieCharacter>(MeshComp->GetOwner());
	if(Pawn)
	{

	}
}

void UAnimNotifyState_ZombieMove::Received_NotifyBegin(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float TotalDuration)
{
	Super::Received_NotifyBegin(MeshComp, Animation, TotalDuration);

	AZombieCharacter* Pawn = Cast<AZombieCharacter>(MeshComp->GetOwner());
	if (Pawn)
	{
		Pawn->GetCharacterMovement()->MaxWalkSpeed = 3.0f;
	}
}

void UAnimNotifyState_ZombieMove::Received_NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation)
{
	Super::Received_NotifyEnd(MeshComp, Animation);

	AZombieCharacter* Pawn = Cast<AZombieCharacter>(MeshComp->GetOwner());
	if (Pawn)
	{
		Pawn->GetCharacterMovement()->MaxWalkSpeed = Pawn->WalkSpeed;
	}
}
