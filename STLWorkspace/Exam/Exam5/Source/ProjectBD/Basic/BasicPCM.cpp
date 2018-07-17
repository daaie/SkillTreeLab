// Fill out your copyright notice in the Description page of Project Settings.

#include "BasicPCM.h"
#include "Basic/MyCharacter.h"
#include "GameFramework/PlayerController.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"


ABasicPCM::ABasicPCM()
{
}

void ABasicPCM::BeginPlay()
{
	Super::BeginPlay();
	AMyCharacter* Pawn = PCOwner ? Cast<AMyCharacter>(PCOwner->GetPawn()) : nullptr;
	if (Pawn)
	{
		SpringArmOffset = Pawn->SpringArm->GetRelativeTransform().GetLocation();
		DefaultFOV = Pawn->bIsIronsight ? IronsightFOV : NormalFOV;
	}
}

void ABasicPCM::UpdateCamera(float DeltaTime)
{
	FVector TargetOffset;
	Super::UpdateCamera(DeltaTime);

	AMyCharacter* Pawn = PCOwner ? Cast<AMyCharacter>(PCOwner->GetPawn()) : nullptr;
	if (Pawn)
	{
		//조준에 모션에 따른 FOV를 조절
		float TargetFOV = Pawn->bIsIronsight ? IronsightFOV : NormalFOV;
		DefaultFOV = FMath::FInterpTo(DefaultFOV, TargetFOV, DeltaTime, 10.0f);
		SetFOV(DefaultFOV);

		//자세에 따른 카메라 위치 보간
		if (Pawn->bIsProne)
		{
			TargetOffset = Pawn->ProneSpringArmPosition;
		}
		else if (Pawn->bIsCrouched)
		{
			TargetOffset = Pawn->CrouchSpringArmPosition;
		}
		else
		{
			TargetOffset = Pawn->NormalSpringArmPosition;
		}
		
		SpringArmOffset = FMath::VInterpTo(SpringArmOffset, TargetOffset, DeltaTime, 5.0f);
		
		Pawn->SpringArm->SetRelativeLocation(SpringArmOffset);
	}
}
