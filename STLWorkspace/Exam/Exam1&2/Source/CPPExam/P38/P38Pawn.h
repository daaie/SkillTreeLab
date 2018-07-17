// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "P38Pawn.generated.h"

UCLASS()
class CPPEXAM_API AP38Pawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AP38Pawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
		void Fire();

	UFUNCTION()
		void MoveForward(float value);

	UFUNCTION()
		void Turn(float value);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class UBoxComponent* Box;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class UStaticMeshComponent* Body;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class UStaticMeshComponent* Left;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class UStaticMeshComponent* Right;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class UFloatingPawnMovement* Movement;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class UArrowComponent* Arrow;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class AP38Rocket> RocketClass;
	
};
