// Fill out your copyright notice in the Description page of Project Settings.

#include "MyGameModeBase.h"
#include "Basic/MyPlayerController.h"
#include "Basic/MyCharacter.h"
#include "ConstructorHelpers.h"
#include "Basic/MyHUD.h"

AMyGameModeBase::AMyGameModeBase()
{
	//PlayerControllerClass = AMyPlayerController::StaticClass(); -> 이건그냥 클래스 받아오는거
	//DefaultPawnClass = AMyCharacter::StaticClass();

	static ConstructorHelpers::FClassFinder<AMyPlayerController>PC_Class(
		TEXT("Blueprint'/Game/Blueprints/MyCharacter/BP_Controller.BP_Controller_C'"));
	if (PC_Class.Succeeded())
	{
		PlayerControllerClass = PC_Class.Class;

	}
	static ConstructorHelpers::FClassFinder<AMyCharacter>Pawn_Class(
		TEXT("Blueprint'/Game/Blueprints/MyCharacter/BP_Character.BP_Character_C'"));
	if (Pawn_Class.Succeeded())
	{
		DefaultPawnClass = Pawn_Class.Class;
	}

	HUDClass = AMyHUD::StaticClass();
}

