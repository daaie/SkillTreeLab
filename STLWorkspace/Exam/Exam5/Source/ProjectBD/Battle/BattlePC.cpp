// Fill out your copyright notice in the Description page of Project Settings.

#include "BattlePC.h"
#include "Battle/BattlePCM.h"
#include "ConstructorHelpers.h"
#include "UI/ItemTooltipWidgetBase.h"
#include "UI/InventoryWidgetBase.h"
#include "Battle/BattleWidgetBase.h"
#include "Kismet/GameplayStatics.h"
#include "BDGameInstance.h"
#include "UnrealNetwork.h"
#include "Battle/BattleGM.h"
#include "Battle/ConclusionWidgetBase.h"
#include "Components/TextBlock.h"
#include "Engine/World.h"

ABattlePC::ABattlePC()
{
	PlayerCameraManagerClass = ABattlePCM::StaticClass();
}

void ABattlePC::BeginPlay()
{
	S2C_SetupWidget();
	UBDGameInstance* GI = Cast<UBDGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	C2S_SetUserID(GI->UserID);
	bIsPawnDead = false;

	ABattleGM* GM = Cast<ABattleGM>(UGameplayStatics::GetGameMode(GetWorld()));
	if (GM)
	{
		GM->C2S_CheckAliveCount();
	}
}


bool ABattlePC::C2S_SetUserID_Validate(const FString & NewUserID)
{
	return true;
}

void ABattlePC::C2S_SetUserID_Implementation(const FString & NewUserID)
{
	UserID = NewUserID;
}

void ABattlePC::S2C_SetupWidget_Implementation()
{
	FStringClassReference ItemTooltipWidgetClass(TEXT("WidgetBlueprint'/Game/Blueprints/UI/ItemlTooltipWidget.ItemlTooltipWidget_C'"));
	if (UClass* MyWidgetClass = ItemTooltipWidgetClass.TryLoadClass<UUserWidget>())
	{
		ItemTooltip = Cast<UItemTooltipWidgetBase>(CreateWidget<UUserWidget>(this, MyWidgetClass));
		ItemTooltip->AddToViewport();
		ItemTooltip->SetVisibility(ESlateVisibility::Collapsed);
	}

	FStringClassReference InventoryWidgetClassRef(TEXT("WidgetBlueprint'/Game/Blueprints/UI/InventoryWidget.InventoryWidget_C'"));
	if (UClass* MyWidgetClass = InventoryWidgetClassRef.TryLoadClass<UUserWidget>())
	{
		InventoryWidget = Cast<UInventoryWidgetBase>(CreateWidget<UUserWidget>(this, MyWidgetClass));
		InventoryWidget->AddToViewport();
		InventoryWidget->SetVisibility(ESlateVisibility::Collapsed);
	}

	FStringClassReference BattleWidgetClassRef(TEXT("WidgetBlueprint'/Game/Blueprints/UI/Battle/BattleWidget.BattleWidget_C'"));
	if (UClass* MyWidgetClass = BattleWidgetClassRef.TryLoadClass<UUserWidget>())
	{
		BattleWidget = Cast<UBattleWidgetBase>(CreateWidget<UUserWidget>(this, MyWidgetClass));
		BattleWidget->AddToViewport();
	}

	FStringClassReference ConclusionWidgetClassRef(TEXT("WidgetBlueprint'/Game/Blueprints/UI/Battle/ConclusionWidget.ConclusionWidget_C'"));
	if (UClass* MyWidgetClass = ConclusionWidgetClassRef.TryLoadClass<UUserWidget>())
	{
		ConclusionWidget = Cast<UConclusionWidgetBase>(CreateWidget<UUserWidget>(this, MyWidgetClass));
		ConclusionWidget->AddToViewport();
		ConclusionWidget->SetVisibility(ESlateVisibility::Collapsed);
	}

	FStringClassReference LoadingWidgetClassRef(TEXT("WidgetBlueprint'/Game/Blueprints/UI/Loading/LoadingWidget.LoadingWidget_C'"));
	if (UClass* MyWidgetClass = LoadingWidgetClassRef.TryLoadClass<UUserWidget>())
	{
		LoadingWidget = Cast<UUserWidget>(CreateWidget<UUserWidget>(this, MyWidgetClass));
		LoadingWidget->AddToViewport();
		LoadingWidget->SetVisibility(ESlateVisibility::Collapsed);
	}

	SetInputMode(FInputModeGameOnly());
}

void ABattlePC::ToggleInventory()
{
	if (InventoryWidget->GetVisibility() == ESlateVisibility::Visible)
	{
		bShowMouseCursor = false;
		SetInputMode(FInputModeGameOnly());
		InventoryWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
	else
	{
		bShowMouseCursor = true;
		SetInputMode(FInputModeGameAndUI());
		InventoryWidget->SetVisibility(ESlateVisibility::Visible);
	}
}

bool ABattlePC::C2S_DestroyItem_Validate(AActor* DestroyItem)
{
	return true;
}

void ABattlePC::C2S_DestroyItem_Implementation(AActor* DestroyItem)
{
	DestroyItem->Destroy();
}

void ABattlePC::S2C_PlayDamageEffect_Implementation()
{
	FStringClassReference BloodEffectWidgetClassRef(TEXT("WidgetBlueprint'/Game/Blueprints/UI/BloodEffect/BloodEffectWidget.BloodEffectWidget_C'"));
	if (UClass* MyWidgetClass = BloodEffectWidgetClassRef.TryLoadClass<UUserWidget>())
	{
		UUserWidget* BloodEffectWidget = Cast<UUserWidget>(CreateWidget<UUserWidget>(this, MyWidgetClass));
		BloodEffectWidget->AddToViewport();
	}
}

void ABattlePC::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ABattlePC, UserID);
	DOREPLIFETIME(ABattlePC, bIsPawnDead);
}

void ABattlePC::S2C_ConclusionChicken_Implementation()
{
	InventoryWidget->RemoveFromParent();
	BattleWidget->RemoveFromParent();

	if (ConclusionWidget && ConclusionWidget->UserID && ConclusionWidget->Message)
	{
		ConclusionWidget->SetVisibility(ESlateVisibility::Visible);
		ConclusionWidget->UserID->SetText(FText::FromString(UserID));
		ConclusionWidget->Message->SetText(FText::FromString(TEXT("이겼닭! 오늘 저녘은 치킨이닭!")));
	}
}

void ABattlePC::S2C_ConclusionDie_Implementation()
{
	InventoryWidget->RemoveFromParent();
	BattleWidget->RemoveFromParent();

	if (ConclusionWidget && ConclusionWidget->UserID && ConclusionWidget->Message)
	{
		ConclusionWidget->SetVisibility(ESlateVisibility::Visible);
		ConclusionWidget->UserID->SetText(FText::FromString(UserID));
		ConclusionWidget->Message->SetText(FText::FromString(TEXT("그럴 수 있어. 이런 날도 있는 거지 뭐.")));
	}
}

void ABattlePC::ReturnLobby()
{
	if (HasAuthority()) //Host인지 확인
	{
		//접속 유저한테 로딩화면 띄우라고 명령
		for (FConstPlayerControllerIterator Iter = GetWorld()->GetPlayerControllerIterator(); Iter; Iter++)
		{
			ABattlePC* PC = Cast<ABattlePC>(*Iter);
			if (PC)
			{
				PC->S2C_ViewLoadingScreen();
			}
		}

		//Host, Client과 동시에 맵 이동 하려면, OpenLevel는 안됨
		GetWorld()->ServerTravel(TEXT("Lobby"));
	}
}


void ABattlePC::S2C_ViewLoadingScreen_Implementation()
{
	if (LoadingWidget)
	{
		LoadingWidget->SetVisibility(ESlateVisibility::Visible);
	}
}



