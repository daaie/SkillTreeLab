// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "P38Pawn.generated.h"

UCLASS()
class P38_API AP38Pawn : public APawn
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

	UFUNCTION(BlueprintCallable, Category = "Rotate")
		void RotatePropeller(UStaticMeshComponent * Mesh);

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Component")
		class UBoxComponent* Box;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Component")
		class UStaticMeshComponent* Body;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Component")
		class UStaticMeshComponent* Left;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Component")
		class UStaticMeshComponent* Right;
	
};
