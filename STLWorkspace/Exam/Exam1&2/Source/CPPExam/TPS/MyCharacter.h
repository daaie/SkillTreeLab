// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MyCharacter.generated.h"

UCLASS()
class CPPEXAM_API AMyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMyCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
		void TryIronsight();

	UFUNCTION()
		void TryCrouch();

	UFUNCTION()
		void MoveRight(float Value);

	UFUNCTION()
		void MoveForward(float Value);

	UFUNCTION()
		void Turn(float Value);

	UFUNCTION()
		void LookUp(float Value);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class UMySceneComponent* Weapon;

	FRotator GetAimOffset() const;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		bool bIsIronsight;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		float JogSpeed = 360.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		float WalkSpeed = 150.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		float SprintSpeed = 1500.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		bool bIsSprint;

	UFUNCTION(BlueprintCallable)
		void Sprint();

	UFUNCTION(BlueprintCallable)
		void UnSprint();

	UFUNCTION(BlueprintCallable)
		void LookAround();

	UFUNCTION(BlueprintCallable)
		void UndoLookAround();
};
