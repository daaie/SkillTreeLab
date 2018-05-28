// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "CharacterZombie.h"
#include "ZombieAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class DAY04_API UZombieAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
	
public:
	UZombieAnimInstance();

	UFUNCTION()
	void NativeUpdateAnimation(float DeltaSeconds);

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		float Speed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
		EZombieState CurrentState;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
		EZombieAnimState CurrentAnimState;
};
