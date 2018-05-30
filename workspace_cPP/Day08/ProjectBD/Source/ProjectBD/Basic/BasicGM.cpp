// Fill out your copyright notice in the Description page of Project Settings.

#include "BasicGM.h"
#include "Basic/BasicPC.h"
#include "Basic/MyCharacter.h"
#include "ConstructorHelpers.h"
#include "Basic/MyHUD.h"

ABasicGM::ABasicGM()
{
	static ConstructorHelpers::FClassFinder<ABasicPC> PC_Class(TEXT("Blueprint'/Game/Blueprints/BP_BasicPC.BP_BasicPC_C'"));
	if (PC_Class.Succeeded())
	{
		//PlayerControllerClass = ABasicPC::StaticClass();
		PlayerControllerClass = PC_Class.Class;
	}

	static ConstructorHelpers::FClassFinder<AMyCharacter> Pawn_Class(TEXT("Blueprint'/Game/Blueprints/Player/BP_MyCharacter.BP_MyCharacter_C'"));
	if (Pawn_Class.Succeeded())
	{
		//DefaultPawnClass = AMyCharacter::StaticClass();
		DefaultPawnClass = Pawn_Class.Class;
		//BP_MyCharacter -=> 사용해라
	}
	

	HUDClass = AMyHUD::StaticClass();
	//class AMyHUD {};
}