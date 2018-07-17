// Fill out your copyright notice in the Description page of Project Settings.

#include "TitlePC.h"
#include "Title/TitleWidgetBase.h"


void ATitlePC::BeginPlay()
{
	FStringClassReference TitleWidgetClassRef(TEXT("WidgetBlueprint'/Game/Blueprints/UI/Title/TitleWidget.TitleWidget_C'"));
	if (UClass* MyWidgetClass = TitleWidgetClassRef.TryLoadClass<UUserWidget>())
	{
		//#include "TitleWidget"
		//TitleWidget = Cast<UTitleWidgetBase>(CreateWidget<UUserWidget>(this, MyWidgetClass));
		TitleWidget = Cast<UTitleWidgetBase>(NewObject<UUserWidget>(this, MyWidgetClass));
		//new

		TitleWidget->AddToViewport();
	}

	FStringClassReference LoadingWidgetClassRef(TEXT("WidgetBlueprint'/Game/Blueprints/UI/Loading/LoadingWidget.LoadingWidget_C'"));
	if (UClass* MyWidgetClass = LoadingWidgetClassRef.TryLoadClass<UUserWidget>())
	{
		LoadingWidget = Cast<UUserWidget>(CreateWidget<UUserWidget>(this, MyWidgetClass));
		LoadingWidget->AddToViewport();
		LoadingWidget->SetVisibility(ESlateVisibility::Collapsed);
	}

	bShowMouseCursor = true;
	SetInputMode(FInputModeUIOnly());
}