// Fill out your copyright notice in the Description page of Project Settings.

#include "BattleGS.h"
#include "UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h" //GetWorld()
#include "Battle/BattlePC.h"
#include "Battle/BattleWidgetBase.h"
#include "Components/TextBlock.h"
#include "Components/ScrollBox.h"
#include "Battle/ConclusionWidgetBase.h"
#include "Battle/BattleGM.h"
#include "Public/TimerManager.h" //GetTimerManger()

ABattleGS::ABattleGS()
{
	SetReplicates(true);
}

void ABattleGS::BeginPlay()
{
	Super::BeginPlay();
}

void ABattleGS::S2A_AddBattleInfo_Implementation(const FString & Message)
{
	ABattlePC* PC = Cast<ABattlePC>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (PC)
	{
		UTextBlock* NewMessage = NewObject<UTextBlock>(PC->BattleWidget->Info);
		if (NewMessage)
		{
			NewMessage->SetJustification(ETextJustify::Right);
			NewMessage->SetText(FText::FromString(Message));
			PC->BattleWidget->Info->AddChild(NewMessage);
			//3.0초 삭제를 만들어서 

			FTimerHandle TimerHandle;
			GetWorld()->GetTimerManager().SetTimer(TimerHandle,
				this, //콜백 함수가 있는 오브젝트
				&ABattleGS::TimerFunction, //콜백 함수 이름
				3.0f, // 실행 시간
				false);
		}
	}
}

void ABattleGS::TimerFunction()
{
	ABattlePC* PC = Cast<ABattlePC>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (PC)
	{
		if (PC->BattleWidget->Info->GetChildrenCount() != 0)
		{
			PC->BattleWidget->Info->RemoveChildAt(0);
		}
	}
}

void ABattleGS::OnRep_AliveCount()
{
	ABattlePC* PC = Cast<ABattlePC>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (PC)
	{
		//UI 작업
		FText Message = FText::FromString(FString::Printf(TEXT("%d 생존"), AliveCount));
		if (PC->BattleWidget)
		{
			PC->BattleWidget->Alive->SetText(Message);
		}
	}
}

void ABattleGS::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ABattleGS, AliveCount);
	DOREPLIFETIME(ABattleGS, PublicMessage);
}

void ABattleGS::OnRep_PublicMessage()
{
	ABattlePC* PC = Cast<ABattlePC>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (PC && PC->ConclusionWidget && PC->ConclusionWidget->PublicMessage)
	{
		PC->ConclusionWidget->PublicMessage->SetText(FText::FromString(PublicMessage));
	}
}