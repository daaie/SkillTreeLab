// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ItemTooltipWidgetBase.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTBD_API UItemTooltipWidgetBase : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;


	// UMG���� �ش��ؽ�Ʈ �������� üũ�ؾ���.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		class UTextBlock* ItemName;
	
};
