// Fill out your copyright notice in the Description page of Project Settings.

#include "BasicPC.h"
#include "Basic/BasicPCM.h"
#include "ConstructorHelpers.h"
#include "UI/ItemTooltipWidgetBase.h"
#include "UI/InventoryWidgetBase.h"

ABasicPC::ABasicPC()
{
	PlayerCameraManagerClass = ABasicPCM::StaticClass();
}

void ABasicPC::BeginPlay()
{
	SetupWidget();
}

void ABasicPC::SetupWidget()
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
}

void ABasicPC::ToggleInventory()
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