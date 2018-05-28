// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "MyAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class DAY04_API UMyAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeUpdateAnimation(float DeltaSeconds);
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		float Direction;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		float Speed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		bool bIsCrouch;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		float AimYaw;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		float AimPitch;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		bool bIsIronsight = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		bool bIsSprint;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		bool bIsProne;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		bool bIsProning;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		bool bIsFire;
	
	UFUNCTION()
		void AnimNotify_ProneStart(UAnimNotify *Notify);

	UFUNCTION()
		void AnimNotify_ProneEnd(UAnimNotify *Notify);

};
