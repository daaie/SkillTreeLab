// Fill out your copyright notice in the Description page of Project Settings.

#include "BasicPC.h"
#include "Basic/BasicPCM.h"
#include "Item/UI/ItemTooltipWidgetBase.h"
#include "Item/UI/InventoryWidgetBase.h"

ABasicPC::ABasicPC()
{
	PlayerCameraManagerClass = ABasicPCM::StaticClass();
}

void ABasicPC::BeginPlay()
{
	FStringClassReference ItemTooltipWidgetClass(TEXT("WidgetBlueprint'/Game/Blueprints/UI/ItemTooltipWidget.ItemTooltipWidget_C'"));
	if (UClass* MyWidgetClass = ItemTooltipWidgetClass.TryLoadClass<UUserWidget>())
	{
		ItemTooltip = Cast<UItemTooltipWidgetBase>(CreateWidget<UUserWidget>(this, MyWidgetClass));
		ItemTooltip->AddToViewport();
		ItemTooltip->SetVisibility(ESlateVisibility::Collapsed);
	}

	FStringClassReference InventoryClass(TEXT("WidgetBlueprint'/Game/Blueprints/UI/InventoryWidget.InventoryWidget_C'"));
	if (UClass* MyWidgetClass = InventoryClass.TryLoadClass<UUserWidget>())
	{
		Inventory = Cast<UInventoryWidgetBase>(CreateWidget<UUserWidget>(this, MyWidgetClass));
		Inventory->AddToViewport();
		Inventory->SetVisibility(ESlateVisibility::Collapsed);
	}

}

void ABasicPC::ToggleInventory()
{
	if (Inventory->GetVisibility() == ESlateVisibility::Visible)
	{
		bShowMouseCursor = false;
		SetInputMode(FInputModeGameOnly());
		Inventory->SetVisibility(ESlateVisibility::Collapsed);
	}
	else
	{
		bShowMouseCursor = true;
		SetInputMode(FInputModeGameAndUI());
		Inventory->SetVisibility(ESlateVisibility::Visible);
	}
}

