// Fill out your copyright notice in the Description page of Project Settings.

#include "MyPlayerCameraManager.h"
#include "MyCharacter.h"
#include "MyPlayerController.h"
#include "Camera/CameraComponent.h"

AMyPlayerCameraManager::AMyPlayerCameraManager()
{

}

void AMyPlayerCameraManager::BeginPlay()
{
	Super::BeginPlay();
	AMyCharacter* Pawn = PCOwner ? Cast<AMyCharacter>(PCOwner->GetPawn()) : nullptr;

	if (Pawn)
	{
		DefaultCameraZ = Pawn->Camera->GetRelativeTransform().GetLocation().Z;
	}
}

//tick °°Àº¾Ö
void AMyPlayerCameraManager::UpdateCamera(float DeltaTime)
{
	AMyCharacter* Pawn = PCOwner ? Cast<AMyCharacter>(PCOwner->GetPawn()) : nullptr;

	if (Pawn)
	{
		float TargetOffset = Pawn->bIsCrouched ? Pawn->CrouchedEyeHeight : 0;


		CrouchOffset = FMath::FInterpTo(CrouchOffset, TargetOffset, DeltaTime, 5.0f);

		FVector CurrentCamaraLocation = Pawn->Camera->GetRelativeTransform().GetLocation();
		FVector NewCamearaLocation = FVector(CurrentCamaraLocation.X, CurrentCamaraLocation.Y, DefaultCameraZ - CrouchOffset);

		//UE_LOG(LogClass, Warning, TEXT("%f"), CrouchOffset);
		Pawn->Camera->SetRelativeLocation(NewCamearaLocation);
	}

	Super::UpdateCamera(DeltaTime);
}
