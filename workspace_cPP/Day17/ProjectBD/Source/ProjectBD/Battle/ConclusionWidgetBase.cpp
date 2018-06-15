// Fill out your copyright notice in the Description page of Project Settings.

#include "ConclusionWidgetBase.h"
#include "Components/TextBlock.h"

void UConclusionWidgetBase::NativeConstruct()
{
	UserID = Cast<UTextBlock>(GetWidgetFromName(TEXT("UserID")));
	Message = Cast<UTextBlock>(GetWidgetFromName(TEXT("Message")));
	PublicMessage = Cast<UTextBlock>(GetWidgetFromName(TEXT("PublicMessage")));
}
