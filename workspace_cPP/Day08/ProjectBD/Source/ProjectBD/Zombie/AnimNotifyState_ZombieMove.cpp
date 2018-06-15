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
		Pawn->GetCharacterMovement()->MaxWalkSpeed = Pawn->WalkSpeed;
	}
}

void UAnimNotifyState_ZombieMove::Received_NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation)
{
	Super::Received_NotifyEnd(MeshComp, Animation);

	AZombieCharacter* Pawn = Cast<AZombieCharacter>(MeshComp->GetOwner());
	if (Pawn)
	{
		if (Pawn->CurrentState == EZombieState::NORMAL)
		{
			//츄적상태에서도 속도가 3이됨
			Pawn->GetCharacterMovement()->MaxWalkSpeed = 3.0f;
		}
	}
}
