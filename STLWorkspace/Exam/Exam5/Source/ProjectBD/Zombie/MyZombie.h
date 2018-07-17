// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MyZombie.generated.h"


UENUM(BlueprintType)
enum class EZombieState : uint8
{
	Normal		UMETA(Display = "Normal"),
	Battle		UMETA(Display = "Battle"),
	Dead		UMETA(Display = "Dead"),
	Chase		UMETA(Display = "Chase")
};

UENUM(BlueprintType)
enum class EZombieAnimState : uint8
{
	Idle		UMETA(Display = "Idle"),
	Walk		UMETA(Display = "Walk"),
	Attack01	UMETA(Display = "Attack01"),
	Attack02	UMETA(Display = "Attack02"),
	Death		UMETA(Display = "Death"),
	Scream		UMETA(Display = "Scream"),
	Run			UMETA(Display = "Run")
};




UCLASS()
class PROJECTBD_API AMyZombie : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMyZombie();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
	void OnSeePawn(APawn * Pawn);

	UFUNCTION()
	void OnHearNoise(APawn* InPawn, const FVector& Location, float Volume);

	float TakeDamage(float DamageAmount, FDamageEvent const & DamageEvent, AController * EventInstigator, AActor * DamageCauser);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Info")
	float CurrentHP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Info")
	float MaxHP = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Info")
	float WalkSpeed = 150.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Info")
	float RunSpeed = 400.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Info")
	float AttackRange = 150.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Info")
	float Attack = 30.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AI")
	class UBehaviorTree* BehaviorTree;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AI")
	class UPawnSensingComponent* PawnSensing;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Info")
	class UWidgetComponent* Widget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	EZombieState CurrentState;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	EZombieAnimState CurrentAnimState;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	TArray<class APatrolPoint*> PatrolPoints;


	void OnAttack();
};
