// Fill out your copyright notice in the Description page of Project Settings.

#include "BasicPC.h"
#include "Basic/BasicPCM.h"
#include "ConstructorHelpers.h"
#include "ItemTooltipWidgetBase.h"

ABasicPC::ABasicPC()
{
	PlayerCameraManagerClass = ABasicPCM::StaticClass();
}

void ABasicPC::BeginPlay()
{
	// 얘느 ㄴ생성자에서만
	//static ConstructorHelpers::FClassFinder<UItemTooltipWidgetBase> Widget_Tooltip

	FStringClassReference ItemTooltipWidgetClass(TEXT("WidgetBlueprint'/Game/Blueprints/UI/ItemTooltipWidget.ItemTooltipWidget_C'"));
	if (UClass* MyWidgetClass = ItemTooltipWidgetClass.TryLoadClass<UItemTooltipWidgetBase>())
	{
		ItemTooltip = Cast<UItemTooltipWidgetBase>(CreateWidget<UUserWidget>(this, MyWidgetClass));
		ItemTooltip->AddToViewport();
		ItemTooltip->SetVisibility(ESlateVisibility::Hidden);
		// collapsed 를 하면 보이지도 않고 걸리지도 않고
		// hiddwen은 보이지만 않고
	}
}
