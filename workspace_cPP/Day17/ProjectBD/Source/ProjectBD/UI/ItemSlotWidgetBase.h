// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Item/ItemDataTable.h"
#include "ItemSlotWidgetBase.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTBD_API UItemSlotWidgetBase : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class UImage* ItemThumb;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class UTextBlock* ItemName;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class UTextBlock* ItemCount;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class UButton* ItemButton;

	class UDataTable* DataTable;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FItemDataTable Data;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool bIsEmpty = true;

	UFUNCTION(BlueprintCallable)
	void SetItem(int NewItemIndex);

	UFUNCTION(BlueprintCallable)
	void ClearItem();

	UFUNCTION(BlueprintCallable)
	bool IsConsume();

	UFUNCTION(BlueprintCallable)
	void AddItemCount(int AddCount);

	bool GetItemTableRow(FItemDataTable& Data);

	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	UFUNCTION(BlueprintCallable)
	void UseItem();
};
