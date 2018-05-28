// Fill out your copyright notice in the Description page of Project Settings.

#include "MyPlayerCameraManager.h"
#include "Basic/MyCharacter.h"
#include "Basic/MyPlayerController.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

AMyPlayerCameraManager::AMyPlayerCameraManager()
{

}

void AMyPlayerCameraManager::BeginPlay()
{
	Super::BeginPlay();
	AMyCharacter* Pawn = PCOwner ? Cast<AMyCharacter>(PCOwner->GetPawn()) : nullptr;

	if (Pawn)
	{
		//DefaultCameraZ = Pawn->Camera->GetRelativeTransform().GetLocation().Z;
		SpringArmOffset = Pawn->SpringArm->GetRelativeTransform().GetLocation();
		DefaultFOV = Pawn->bIsIronsight ? IronsightFOV : NormalFOV;
	}
}

//tick 같은애
void AMyPlayerCameraManager::UpdateCamera(float DeltaTime)
{
	FVector TargetOffset ;
	Super::UpdateCamera(DeltaTime);
	// 가끔 PCOwner 가 NULL이라서 처리해줘야함.
	AMyCharacter* Pawn = PCOwner ? Cast<AMyCharacter>(PCOwner->GetPawn()) : nullptr;

	if (Pawn)
	{
		float TargetFOV = Pawn->bIsIronsight ? IronsightFOV : NormalFOV;
		DefaultFOV = FMath::FInterpTo(DefaultFOV, TargetFOV, DeltaTime, 5.0f);
		SetFOV(DefaultFOV);

		if (Pawn->bIsCrouched)
		{
			TargetOffset = Pawn->CrouchSpringArmPosition;
;		}
		else if(Pawn->bIsProne)
		{
			TargetOffset = Pawn->ProneSpringArmPosition;
		}
		else
		{
			TargetOffset = Pawn->NormalSpringArmPosition;
		}

		//CrouchOffset = FMath::FInterpTo(CrouchOffset, TargetOffset, DeltaTime, 5.0f);
		SpringArmOffset = FMath::VInterpTo(SpringArmOffset,TargetOffset,DeltaTime,5.0f);
		//FVector CurrentSpringArm = Pawn->SpringArm->GetRelativeTransform().GetLocation();

		//Pawn->Camera->SetRelativeLocation(FVector(0,0,TargetOffset));
		Pawn->SpringArm->SetRelativeLocation(SpringArmOffset);

		//FVector CurrentCamaraLocation = Pawn->Camera->GetRelativeTransform().GetLocation();
		//FVector NewCamearaLocation = FVector(CurrentCamaraLocation.X, CurrentCamaraLocation.Y, DefaultCameraZ - CrouchOffset);

		////UE_LOG(LogClass, Warning, TEXT("%f"), CrouchOffset);
		//Pawn->Camera->SetRelativeLocation(NewCamearaLocation);
	}

}
