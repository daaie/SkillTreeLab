// Fill out your copyright notice in the Description page of Project Settings.

#include "AnimNotifyState_ZombieMove.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Zombie/MyZombie.h"

void UAnimNotifyState_ZombieMove::NotifyTick(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float FrameDeltaTime)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime);

	AMyZombie* Pawn = Cast<AMyZombie>(MeshComp->GetOwner());
	if (Pawn)
	{

	}
}

void UAnimNotifyState_ZombieMove::NotifyBegin(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float TotalDuration)
{

	AMyZombie* Pawn = Cast<AMyZombie>(MeshComp->GetOwner());
	if (Pawn)
	{
		if (Pawn->CurrentState == EZombieState::Normal)
		{
			Pawn->GetCharacterMovement()->MaxWalkSpeed = Pawn->WalkSpeed;
		}
	}
}

void UAnimNotifyState_ZombieMove::NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation)
{

	AMyZombie* Pawn = Cast<AMyZombie>(MeshComp->GetOwner());
	if (Pawn)
	{
		if (Pawn->CurrentState == EZombieState::Normal) 
		{
			//비헤이비어 트리에서 걷기중에 상태가 바뀌어도 애님 노티파이 스테이트가 실행되서 
			//추적 상태에서도 속도가 3이 됨.
			Pawn->GetCharacterMovement()->MaxWalkSpeed = 3.0f;
		}
	}
}
