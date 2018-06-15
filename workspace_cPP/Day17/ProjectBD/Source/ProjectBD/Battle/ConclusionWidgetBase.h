// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ConclusionWidgetBase.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTBD_API UConclusionWidgetBase : public UUserWidget
{
	GENERATED_BODY()
public:
	class UTextBlock* UserID;
	class UTextBlock* Message;
	class UTextBlock* PublicMessage;

	virtual void NativeConstruct() override;
};
