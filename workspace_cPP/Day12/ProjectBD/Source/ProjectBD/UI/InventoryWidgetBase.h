// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryWidgetBase.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTBD_API UInventoryWidgetBase : public UUserWidget
{
	GENERATED_BODY()
	
	
public:
	virtual void NativeConstruct() override;

	class UScrollBox* InventoryScrollBox;

	TArray<class UItemSlotWidgetBase*> Slots;

	UItemSlotWidgetBase* GetEmptySlot();
	UItemSlotWidgetBase* GetSameIDSlot(int SameItemIndex);
	
};
