// Fill out your copyright notice in the Description page of Project Settings.

#include "BattleWidgetBase.h"
#include "Components/ProgressBar.h"
#include "Components/ScrollBox.h"
#include "Components/TextBlock.h"
#include "Battle/BattleCharacter.h"

void UBattleWidgetBase::NativeConstruct()
{
	HPBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("HPBar")));
	Alive = Cast<UTextBlock>(GetWidgetFromName(TEXT("Alive")));
	Info = Cast<UScrollBox>(GetWidgetFromName(TEXT("Info")));
}

void UBattleWidgetBase::NativeTick(const FGeometry & MyGeometry, float InDeltaTime)
{
	ABattleCharacter* Pawn = Cast<ABattleCharacter>(GetOwningPlayerPawn());

	if(Pawn)
	{
		HPBar->SetPercent(Pawn->CurrentHP / Pawn->MaxHP);
	}
}
