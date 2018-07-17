// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Http.h"
#include "TitlePC.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTBD_API ATitlePC : public APlayerController
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;

	class UTitleWidgetBase* TitleWidget;

	class UUserWidget* LoadingWidget;
	
	class FHttpModule* HttpConnection;

	void LoginHTTPGet(const FString& ID, const FString& Password);

	void LoginHTTPPost(const FString& ID, const FString& Password);
	
	//UFUNCTION()
	void OnRequestComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccess);
};
