// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CharacterZombie.generated.h"



UENUM(BlueprintType)
enum class EZombieState : uint8
{
	NORMAL	UMETA(Display = "Normal"),
	BATTLE	UMETA(Display = "Battle"),
	DEAD	UMETA(Display = "Dead"),
	CHASE	UMETA(Display = "Chase"),
};

UENUM(BlueprintType)
enum class EZombieAnimState : uint8
{
	Idle	UMETA(Display = "Idle"),
	Walk	UMETA(Display = "Walk"),
	Attack01	UMETA(Display = "Attack01"),
	Attack02	UMETA(Display = "Attack02"),
	Death	UMETA(Display = "Death"),
	Scream	UMETA(Display = "Scream"),
	Run	UMETA(Display = "Run"),
};

UCLASS()
class DAY04_API ACharacterZombie : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACharacterZombie();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual float TakeDamage(float DamageAmount, FDamageEvent const & DamageEvent, AController * EventInstigator, AActor * DamageCauser) override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Info")
		float CurrentHP;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Info")
		float MaxHP=100;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AI")
		class UBehaviorTree *BehaviorTree;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
		EZombieState CurrentState;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
		EZombieAnimState CurrentAnimState;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
		TArray<class AMyTargetPoint*> PatrolPoints;
	//TArray -> StlÀÇ vector³ª list
};
