// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Zombie/MyZombie.h"
#include "ZombieAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTBD_API UZombieAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	EZombieState CurrentState;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	EZombieAnimState CurrentAnimState;
	
	virtual void NativeUpdateAnimation(float DeltaSeconds);

	UFUNCTION()
	void AnimNotify_OnAttack(UAnimNotify* Notify);
};
