// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "ZombieCharacter.h"
#include "ZombieAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTBD_API UZombieAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
	
public:
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
		EZombieState CurrentState;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
		EZombieAnimState CurrentAnimState;

	UFUNCTION()
	void AnimNotify_StartAttack(UAnimNotify* Notify);
};
