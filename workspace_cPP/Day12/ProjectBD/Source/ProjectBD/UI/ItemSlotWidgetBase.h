// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ItemDataTable.h"
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UImage* ItemThumb;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UTextBlock* ItemName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UTextBlock* ItemCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UButton* ItemButton;

	class UDataTable* DataTable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FItemDataTable Data;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
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
	void ItemUse();

};
