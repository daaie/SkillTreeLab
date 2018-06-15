// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BattleWidgetBase.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTBD_API UBattleWidgetBase : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime);

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class UProgressBar* HPBar;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class UScrollBox* Info;


	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class UTextBlock* Alive;

	
	
	
};
